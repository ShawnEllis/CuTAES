#include "Database.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Student.h"

Database* Database::m_pInstance = 0;

Database::Database() {
    m_pStudentQueue = new Queue<Student*>();
    m_pApplicationQueue = new Queue<Queue<TaApplication*>*>();
}

Database::~Database() {
    
}

void Database::addStudent(Student *pStudent) {
    m_pStudentQueue->pushBack(pStudent);
}

void Database::addApplication(TaApplication *pApplication) {
    Queue<TaApplication*>* courseQueue = findQueueForCourse(pApplication->getCourse());
    if (courseQueue == 0) {
        courseQueue = new Queue<TaApplication*>();
        m_pApplicationQueue->pushBack(courseQueue);
    }
    courseQueue->pushBack(pApplication);
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

Queue<TaApplication*>* Database::findQueueForCourse(const std::string &course) {
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

Database* Database::instance() {
    if (m_pInstance == 0) {
        m_pInstance = new Database();
    }
    return m_pInstance;
}