#include "Database.h"

#include "Queue.h"

Database* Database::m_pInstance = 0;

Database::Database() {
    m_pStudentQueue = new Queue<Student*>();
    m_pApplicationQueue = new Queue<TaApplication*>();
}

Database::~Database() {
    
}

void Database::addStudent(Student *pStudent) {
    m_pStudentQueue->pushBack(pStudent);
}

void Database::addApplication(TaApplication *pApplication) {
    m_pApplicationQueue->pushBack(pApplication);
}

Database* Database::instance() {
    if (m_pInstance == 0) {
        m_pInstance = new Database();
    }
    return m_pInstance;
}