#include "TaApplication.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>

#include "Queue.h"
#include "CuTAES.h"

TaApplication::TaApplication(const std::string& course, const std::string& studentID) {
    m_course = course;
    m_studentID = studentID;
    m_pRelatedCourses = new Queue<RelatedCourse>();
    m_pTaCourses = new Queue<TaCourse>();
}

void TaApplication::addRelatedCourse(const std::string &course, int year, char term, const std::string &grade) {
    m_pRelatedCourses->pushBack((RelatedCourse) {course, year, term, grade});
}

void TaApplication::addTaCourse(const std::string &course, int year, char term, const std::string &supervisor) {
    m_pTaCourses->pushBack((TaCourse) {course, year, term, supervisor});
}

void TaApplication::saveToFile() {
    //Create filename using system time
    std::stringstream filename;
    filename << CuTAES::instance()->getDataDirectory() << "application/app_" << m_course << "_" << time(NULL) << ".txt";
    
    std::ofstream file;
    file.open(filename.str().data());
    if (file.is_open()) {
        file << m_course << std::endl;
        file << m_studentID << std::endl;
        file << "pending" << std::endl; //TODO: use actual val
        file << "related" << std::endl;        
        {
            //Save related courses
            Node<RelatedCourse> *pCur = m_pRelatedCourses->front();
            while (pCur != 0) {
                file << "  " << pCur->value.m_course << std::endl;
                file << "  " << pCur->value.m_year << std::endl;
                file << "  " << pCur->value.m_term << std::endl;
                file << "  " << pCur->value.m_grade << std::endl;
                pCur = pCur->m_pNext;
            }
        }
        file << "ta" << std::endl;
        {
            //Save TA'd courses
            Node<TaCourse> *pCur = m_pTaCourses->front();
            while (pCur != 0) {
                file << "  " << pCur->value.m_course << std::endl;
                file << "  " << pCur->value.m_year << std::endl;
                file << "  " << pCur->value.m_term << std::endl;
                file << "  " << pCur->value.m_supervisor << std::endl;
                pCur = pCur->m_pNext;
            }
        }
        file << "end" << std::endl;
        file.flush();
        file.close();
    }
}