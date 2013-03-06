#include "TaApplication.h"
#include <fstream>
#include <iostream>

#include "Queue.h"
#include "CuTAES.h"

TaApplication::TaApplication(const std::string& course, const std::string& studentID) {
    m_course = course;
    m_studentID = studentID;
    m_status = "pending";
    m_pRelatedCourses = new Queue<RelatedCourse>();
    m_pTaCourses = new Queue<TaCourse>();
    m_pWorkExperience = new Queue<WorkExperience>();
}

void TaApplication::addRelatedCourse(const std::string &course, int year, char term, const std::string &grade) {
    m_pRelatedCourses->pushBack((RelatedCourse) {course, year, term, grade});
}

void TaApplication::addTaCourse(const std::string &course, int year, char term, const std::string &supervisor) {
    m_pTaCourses->pushBack((TaCourse) {course, year, term, supervisor});
}

void TaApplication::addWorkExperience(const std::string &job, const std::string &descr, const std::string &start, const std::string &end) {
    m_pWorkExperience->pushBack((WorkExperience) {job, descr, start, end});
}

void TaApplication::saveToFile(const std::string& filename) {
    //Create filename using system time
    
    
    std::ofstream file;
    file.open(filename.data());
    if (file.is_open()) {
        file << m_course << std::endl;
        file << m_studentID << std::endl;
        file << m_status << std::endl;
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
        file << "work" << std::endl;
        {
            //Save work experience
            Node<WorkExperience> *pCur = m_pWorkExperience->front();
            while (pCur != 0) {
                file << "  " << pCur->value.m_job << std::endl;
                file << "  " << pCur->value.m_description << std::endl;
                file << "  " << pCur->value.m_start << std::endl;
                file << "  " << pCur->value.m_end << std::endl;
                pCur = pCur->m_pNext;
            }
        }
        file << "end" << std::endl;
        file.flush();
        file.close();
    }
}