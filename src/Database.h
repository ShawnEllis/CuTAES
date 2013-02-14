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

template <class T>
class Queue;
class Student;
class TaApplication;

class Database {
public:
    virtual ~Database();
    
    void addStudent(Student *pStudent);
    void addApplication(TaApplication *pApplication);
    
    static Database *instance();
    
private:
    Database();
    
    Queue<Student*> *m_pStudentQueue;
    Queue<TaApplication*> *m_pApplicationQueue;
    
    void loadStudents();
    void loadApplications();
    
    static Database *m_pInstance;
};

#endif //DATABASE_H
