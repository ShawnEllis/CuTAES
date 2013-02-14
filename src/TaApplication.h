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
    TaApplication(const std::string& m_course);
    
    void addRelatedCourse(const std::string& course, int year, char term, const std::string& grade);
    void addTaCourse(const std::string& course, int year, char term, const std::string& supervisor);
    
    void saveToFile();
    
    const std::string& getCourse() {return m_course;}
    
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
    
private:
    std::string m_course;
    
    Queue<RelatedCourse> *m_pRelatedCourses;
    Queue<TaCourse> *m_pTaCourses;
    
};

#endif //TA_APPLICATION_H
