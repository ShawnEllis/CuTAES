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

template <class T>
class Queue;
class Student;
class TaApplication;

class Database {
public:
    virtual ~Database();
    
    void getCourses(const std::string** courses, int& count) {*courses = m_courses; count = m_numCourses;}
    
    void addStudent(Student *pStudent, bool save=true);
    void addApplication(TaApplication *pApplication, bool save=true);
    
    Student* getStudent(const std::string& stuID);
    Queue<TaApplication*>* getApplications(const std::string& str);
    
    void replaceStudent(Student *pExisting, Student *pNew);
    
    static Database *instance();
    
private:
    Database();
    
    Queue<Student*> *m_pStudentQueue;
    Queue<Queue<TaApplication*>*>* m_pApplicationQueue;
    
    std::string* m_courses;
    int m_numCourses;
    
    void loadCourses();
    void loadStudents();
    void loadStudent(const std::string &filename);
    void loadApplications();
    void loadApplication(const std::string &filename);
    
    static Database *m_pInstance;
};

#endif //DATABASE_H
