/*
 *  Database.h
 *
 *  Loads saved students and applications from file,
 *  and stores them in queues.
 *  Provides methods for getting data from the database.
 *
 */

#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include "TaApplication.h"

template <class T>
class Queue;
class Student;

class Database {
public:
    virtual ~Database();
    
    void getCourses(const std::string** courses, int& count) {*courses = m_courses; count = m_numCourses;}
    void getResearchAreas(const std::string** researchAreas, int& count) {*researchAreas = m_researchAreas; count = m_numResearchAreas;}
    
    void addStudent(Student *pStudent, bool save=true);
    void addApplication(TaApplication *pApplication, bool save=true);
    
    Student* getStudent(const std::string& stuID);
    Queue<Student*>* getStudents() { return m_pStudentQueue; }
    Queue<TaApplication*>* getApplications(const std::string& str);
    Queue<TaApplication*>* getApplications(const std::string& str, ApplicationStatus appStatus);
    TaApplication* findApplication(const std::string& strID);
    
    void replaceStudent(Student *pExisting, Student *pNew);
    void replaceApplication(TaApplication *pExisting, TaApplication *pNew);
    
    static Database *instance();
    
private:
    Database();
    
    Queue<Student*>* m_pStudentQueue;
    Queue<Queue<TaApplication*>*>* m_pApplicationQueue;
    
    std::string *m_courses, *m_researchAreas;
    int m_numCourses, m_numResearchAreas;
    
    void loadList(const std::string&, std::string**, int&);
    void loadStudents();
    void loadStudent(const std::string &filename);
    void loadApplications();
    void loadApplication(const std::string &filename);
    
    static Database *m_pInstance;
};

#endif //DATABASE_H
