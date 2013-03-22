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

template<class T>
class Node;

enum ApplicationStatus {
    STATUS_PENDING,
    STATUS_ACCEPTED,
    STATUS_CLOSED
};

class TaApplication {
public:
    TaApplication(const std::string& course, const std::string& studentID, const std::string& status = "pending", const std::string& appID="");
    
    void addRelatedCourse(const std::string& course, int year, char term, const std::string& grade);
    void addTaCourse(const std::string& course, int year, char term, const std::string& supervisor);
    void addWorkExperience(const std::string& job, const std::string& descr, const std::string& start, const std::string& end);
    
    void saveToFile();
    
    const std::string& getCourse() {return m_course;}
    const std::string& getStudentID() {return m_studentID;}
    const std::string& getApplicationID() {return m_applicationID;}
    
    ApplicationStatus getStatus() {return m_status;}
    void setStatus(ApplicationStatus st) {m_status = st;}
    
    const std::string getStrStatus() {
        switch(m_status) {
            case STATUS_PENDING: return "pending";
            case STATUS_ACCEPTED: return "accepted";
            case STATUS_CLOSED: return "closed";
        }
    }
    
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
    
    Node<RelatedCourse>* getRelatedCourses();
    Node<TaCourse>* getTaCourses();
    Node<WorkExperience>* getWorkExperience();
    
    bool operator==(TaApplication& other) {
        return (this->m_course.compare(other.m_course) == 0
            && this->m_studentID.compare(other.m_studentID) == 0
            && this->m_applicationID.compare(other.m_applicationID) == 0);
    }
    
    bool operator!=(TaApplication& other) {
        return !(this->operator==(other));
    }
    
private:
    std::string m_course;
    std::string m_studentID;
    std::string m_applicationID;
    ApplicationStatus m_status;
    
    Queue<RelatedCourse> *m_pRelatedCourses;
    Queue<TaCourse> *m_pTaCourses;
    Queue<WorkExperience> *m_pWorkExperience;
    
};

#endif //TA_APPLICATION_H
