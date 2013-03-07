#include "Database.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <time.h>

#include "CuTAES.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Student.h"
#include "UndergradStudent.h"
#include "GradStudent.h"

#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

Database* Database::m_pInstance = 0;

Database::Database() {
    m_pStudentQueue = new Queue<Student*>();
    m_pApplicationQueue = new Queue<Queue<TaApplication*>*>();
    loadCourses();
    loadStudents();
    loadApplications();
}

Database::~Database() {
    
}

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
        std::stringstream appName, appFilename;
        appName << "app_" << pApplication->getCourse() << "_" << time(NULL) << ".txt";
        appFilename << CuTAES::instance()->getDataDirectory() << "application/" << appName.str();
        pApplication->saveToFile(appFilename.str());
        
        //Add to application list
        std::string filename = CuTAES::instance()->getDataDirectory() + "applications.txt";
        std::ofstream file;
        file.open(filename.data(), std::fstream::out | std::fstream::app);
        if (file.is_open()) {
            file << appName.str() << std::endl;
            file.close();
        }
    }
}

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

void Database::loadCourses() {
    //Load course list
    Queue<std::string>* pQueue = new Queue<std::string>();
    std::ifstream file;
    file.open((CuTAES::instance()->getDataDirectory() + "CourseList.txt").data());
    if (file.is_open()) {
        std::string line;
        //Read each course and make buttons for each
        while (file.good()) {
            getline(file, line);
            pQueue->pushBack(line);
        }
        file.close();
    }
    m_courses = pQueue->toArray();
    m_numCourses = pQueue->getSize();
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
        std::string course, stuNum;
        getline(file, course);
        getline(file, stuNum);
        addApplication(new TaApplication(course, stuNum), false);
#ifdef DEBUG
        dout << "Loaded application " << course << " " << stuNum << std::endl;
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