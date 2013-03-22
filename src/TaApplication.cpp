#include "TaApplication.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>

#include "Queue.h"
#include "CuTAES.h"

TaApplication::TaApplication(const std::string& course, const std::string& studentID, const std::string& status, const std::string& appID) {
    m_course = course;
    m_studentID = studentID;
    if (status.compare("accepted") == 0) {
        m_status = STATUS_ACCEPTED;
    } else if (status.compare("closed") == 0) {
        m_status = STATUS_CLOSED;        
    } else {
        m_status = STATUS_PENDING;
    }
    m_pRelatedCourses = new Queue<RelatedCourse>();
    m_pTaCourses = new Queue<TaCourse>();
    m_pWorkExperience = new Queue<WorkExperience>();
    if (appID.compare("") == 0) {
        std::stringstream id;
        id << "app_" << m_course << "_" << time(NULL);
        m_applicationID = id.str();
    } else {
        m_applicationID = appID;
    }
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

Node<TaApplication::RelatedCourse>* TaApplication::getRelatedCourses() {
    return m_pRelatedCourses->front();
}

Node<TaApplication::TaCourse>* TaApplication::getTaCourses() {
    return m_pTaCourses->front();
}

Node<TaApplication::WorkExperience>* TaApplication::getWorkExperience() {
    return m_pWorkExperience->front();
}

void TaApplication::saveToFile() {
    std::string filename = CuTAES::instance()->getDataDirectory() + "application/" + m_applicationID + ".txt";
    std::ofstream file;
    file.open(filename.data());
    if (file.is_open()) {
        file << m_applicationID << std::endl;
        file << m_course << std::endl;
        file << m_studentID << std::endl;
        file << getStrStatus() << std::endl;
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