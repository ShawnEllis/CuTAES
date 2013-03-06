/*
 *  TaApplication.h
 *
 *  Represents a TA application.
 *
 */

#ifndef TA_APPLICATION_H
#define TA_APPLICATION_H

#include <string>

template<class T>
class Queue;

class TaApplication {
public:
    TaApplication(const std::string& course, const std::string& studentID);
    
    void addRelatedCourse(const std::string& course, int year, char term, const std::string& grade);
    void addTaCourse(const std::string& course, int year, char term, const std::string& supervisor);
    void addWorkExperience(const std::string& job, const std::string& descr, const std::string& start, const std::string& end);
    
    void saveToFile(const std::string &filename);
    
    const std::string& getCourse() {return m_course;}
    
    const std::string& getStudentID() {return m_studentID;}
    
    struct RelatedCourse {
        std::string m_course;
        int m_year;
        char m_term;
        std::string m_grade;
    };
    
    struct TaCourse {
        std::string m_course;
        int m_year;
        char m_term;
        std::string m_supervisor;
    };
    
    struct WorkExperience {
        std::string m_job;
        std::string m_description;
        std::string m_start;
        std::string m_end;
    };
    
private:
    std::string m_course;
    std::string m_studentID;
    std::string m_status;
    
    Queue<RelatedCourse> *m_pRelatedCourses;
    Queue<TaCourse> *m_pTaCourses;
    Queue<WorkExperience> *m_pWorkExperience;
    
};

#endif //TA_APPLICATION_H
