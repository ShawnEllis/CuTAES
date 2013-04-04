#include "Database.h"
#include <string>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ncurses.h>

#include "CuTAES.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Student.h"
#include "UndergradStudent.h"
#include "GradStudent.h"
#include "StringUtil.h"

#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

Database* Database::m_pInstance = 0;

Database::Database() {
    m_pStudentQueue = new Queue<Student*>();
    m_pApplicationQueue = new Queue<Queue<TaApplication*>*>();
    loadList("CourseList.txt", &m_courses, m_numCourses);
    loadList("ResearchAreaList.txt", &m_researchAreas, m_numResearchAreas);
    loadStudents();
    loadApplications();
}

Database::~Database() {
    
}

/*
 * Accessors
 */

Student* Database::getStudent(const std::string &stuID) {
    Node<Student*>* curNode = m_pStudentQueue->front();
    while (curNode != 0) {
        if (curNode->value != 0 && stuID.compare(curNode->value->getStudentID()) == 0) {
            return  curNode->value;
        }
        curNode = curNode->m_pNext;
    }
    return 0;
}

Queue<TaApplication*>* Database::getApplications(const std::string &course) {
    Node<Queue<TaApplication*>*>* curNode = m_pApplicationQueue->front();
    Queue<TaApplication*>* curQueue = 0;
    //Find the queue for the course
    while (curNode != 0) {
        curQueue = curNode->value;
        if (curQueue != 0) {
            if (curQueue->peek() != 0) {
                if (curQueue->peek()->getCourse().compare(course) == 0) {
                    return curQueue;
                }
            }
        }
        curNode = curNode->m_pNext;
    }
    return 0;
}

Queue<TaApplication*>* Database::getApplications(const std::string &course, ApplicationStatus appStatus) {
    Queue<TaApplication*>* pApplications = getApplications(course);
    if (pApplications != 0) {
        Queue<TaApplication*>* pTrimmedApplications = new Queue<TaApplication*>();
        Node<TaApplication*>* pApp = pApplications->front();
        while (pApp != 0) {
            if (pApp->value->getStatus() == appStatus) {
                pTrimmedApplications->pushBack(pApp->value);
            }
            pApp = pApp->m_pNext;
        }
        return pTrimmedApplications;
    }
    return 0;
}

TaApplication* Database::findApplication(const std::string& strID) {
    //Get course
    std::string strCourse = strID.substr(4, 8);
    Queue<TaApplication*>* pApplications = getApplications(strCourse);
    if (pApplications != 0) {
        Node<TaApplication*>* pApp = pApplications->front();
        while (pApp != 0) {
            if (pApp->value->getApplicationID().compare(strID) == 0) {
                return pApp->value;
            }
            pApp = pApp->m_pNext;
        }
    }
    return 0;
}

/*
 * Database addition / editing / removal
 */

void Database::addStudent(Student *pStudent, bool save) {
    m_pStudentQueue->pushBack(pStudent);
    if (save) {
        pStudent->saveToFile();
        //Add to student list
        std::string filename = CuTAES::instance()->getDataDirectory() + "students.txt";
        std::ofstream file;
        file.open(filename.data(), std::fstream::out | std::fstream::app);
        if (file.is_open()) {
            file << pStudent->getStudentID() << ".txt" << std::endl;
            file.close();
        }
    }
}

void Database::addApplication(TaApplication *pApplication, bool save) {
    Queue<TaApplication*>* courseQueue = getApplications(pApplication->getCourse());
    if (courseQueue == 0) {
        courseQueue = new Queue<TaApplication*>();
        m_pApplicationQueue->pushBack(courseQueue);
    }
    courseQueue->pushBack(pApplication);
    
    if (save) {
        pApplication->saveToFile();
        //Add to application list
        std::string filename = CuTAES::instance()->getDataDirectory() + "applications.txt";
        std::ofstream file;
        file.open(filename.data(), std::fstream::out | std::fstream::app);
        if (file.is_open()) {
            file << pApplication->getApplicationID() << ".txt" << std::endl;
            file.close();
        }
    }
}

void Database::replaceStudent(Student *pExisting, Student *pNew) {
    if (pExisting == 0 || pNew == 0) {
        return;
    }
    //Update db
    Node<Student*>* pCur = m_pStudentQueue->front();
    while (pCur != 0) {
        if (pCur->value->getStudentID() == pExisting->getStudentID()) {
            //Student was found in DB, replace
            pNew->saveToFile(); //Overwrites file
            pCur->value = pNew;
            delete pExisting;
        }
        pCur = pCur->m_pNext;
    }
}

void Database::replaceApplication(TaApplication *pExisting, TaApplication *pNew) {
    if (pExisting == 0 || pNew == 0 || pExisting->getCourse().compare(pNew->getCourse()) != 0) {
        return;
    }
    //Update db
    Queue<TaApplication*>* pApplications = getApplications(pExisting->getCourse());
    Node<TaApplication*>* pCur = pApplications->front();
    while (pCur != 0) {
        if (*(pCur->value) == *pExisting) {
            pNew->saveToFile();
            pCur->value = pNew;
            delete pExisting;
            return;
        }
        pCur = pCur->m_pNext;
    }
}

/*
 * Loading related methods
 */

void Database::loadList(const std::string& fileName, std::string** pList, int& pCount) {
    //Load list of strings
    Queue<std::string>* pQueue = new Queue<std::string>();
    std::ifstream file;
    file.open((CuTAES::instance()->getDataDirectory() + fileName).data());
    if (file.is_open()) {
        std::string line;
        //Read each line and add to queue
        while (file.good()) {
            getline(file, line);
            pQueue->pushBack(line);
        }
        file.close();
    }
    *pList = pQueue->toArray();
    pCount = pQueue->getSize();
    delete pQueue;
}

void Database::loadStudents() {
#ifdef DEBUG
    dout << "Loading students..." << std::endl;
#endif //DEBUG
    std::ifstream file;
    file.open((CuTAES::instance()->getDataDirectory() + "students.txt").data());
    if (file.is_open()) {
        std::string line;
        while (file.good()) {
            getline(file, line);
            if (line.length() == 0) {
                break;
            }
            loadStudent(CuTAES::instance()->getDataDirectory() + "student/" + line);
        }
        file.close();
    }
#ifdef DEBUG
    dout << "Loaded students." << std::endl;
#endif //DEBUG
}

void Database::loadStudent(const std::string &filename) {
#ifdef DEBUG
    dout << "Loading student " << filename << std::endl;
#endif //DEBUG
    std::ifstream file;
    file.open(filename.data());
    if (file.is_open()) {
        std::string *data = new std::string[8];
        std::string line;
        std::string stuType;
        getline(file, stuType);
        int i = 0;
        while (file.good() && i < 8) {
            getline(file, line);
            data[i] = line;
            i++;
        }
        if (stuType.compare("undergrad") == 0) {
            addStudent(new UndergradStudent(data), false);   
        } else if (stuType.compare("grad") == 0) {
            addStudent(new GradStudent(data), false);   
        }
#ifdef DEBUG
        dout << "Loaded student " << data[2] << std::endl;
#endif //DEBUG
        file.close();
    }
}

void Database::loadApplications() {
#ifdef DEBUG
    dout << "Loading applications..." << std::endl;
#endif //DEBUG
    std::ifstream file;
    file.open((CuTAES::instance()->getDataDirectory() + "applications.txt").data());
    if (file.is_open()) {
        std::string line;
        while (file.good()) {
            getline(file, line);
            if (line.length() == 0) {
                break;
            }
            loadApplication(CuTAES::instance()->getDataDirectory() + "application/" + line);
        }
        file.close();
    }
#ifdef DEBUG
    dout << "Loaded applications." << std::endl;
#endif //DEBUG
}

void Database::loadApplication(const std::string &filename) {
#ifdef DEBUG
    dout << "Loading application " << filename << std::endl;
#endif //DEBUG
    std::ifstream file;
    file.open(filename.data());
    if (file.is_open()) {
        std::string line1, line2, line3, line4;
        getline(file, line1); //appID
        getline(file, line2); //course
        getline(file, line3); //stuID
        getline(file, line4); //status
        TaApplication* pApp = new TaApplication(line2, line3, line4, line1);
        addApplication(pApp, false);
        //get related and TA'd courses
        getline(file, line1); //throw away 'related'
        char parseType = 'r';
        while (true) {
            getline(file, line1); //course
            StringUtil::trim(line1);
            if (line1.compare("ta") == 0) {
                parseType = 't';
                continue;
            } else if (line1.compare("work") == 0) {
                parseType = 'w';
                continue;                
            } else if (line1.compare("end") == 0) {
                break;
            }
            getline(file, line2); //Year
            StringUtil::trim(line2);
            getline(file, line3); //Term
            StringUtil::trim(line3);
            getline(file, line4); //Grade
            StringUtil::trim(line4);
            if (parseType == 'r') {
                pApp->addRelatedCourse(line1, atoi(line2.data()), line3[0], line4);
            } else if (parseType == 't') {
                pApp->addTaCourse(line1, atoi(line2.data()), line3[0], line4);                
            } else {
                pApp->addWorkExperience(line1, line2, line3, line4);
            }
        }
        //Add reference to application to student
        Student *pStu = getStudent(pApp->getStudentID());
        if (pStu == 0) {
#ifdef DEBUG
            dout << "Student " << pApp->getStudentID() << " not found. Database is invalid." << std::endl;
#endif //DEBUG            
            exit(-1);
        }
        pStu->addApplication(pApp);
#ifdef DEBUG
        dout << "Loaded application " << pApp->getCourse() << " " << pApp->getStudentID() << std::endl;
#endif //DEBUG
        file.close();
    }
}

Database* Database::instance() {
    if (m_pInstance == 0) {
        m_pInstance = new Database();
    }
    return m_pInstance;
}