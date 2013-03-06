#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <fstream>
#include <iostream>

template<class T>
class Queue;
class TaApplication;

enum StudentType {
    TYPE_UNDERGRAD,
    TYPE_GRAD
};

class Student {
public:
    virtual ~Student() {};

	void saveToFile();
    virtual int getData(std::string**) = 0;
    
    void addApplication(TaApplication *pApp);
    bool hasAppliedForCourse(const std::string& course);
    
    bool equals(Student *pStu) {return m_firstName.compare(pStu->getFirstName()) == 0
                                    && m_lastName.compare(pStu->getLastName()) == 0
                                    && m_studentID.compare(pStu->getStudentID()) == 0
                                    && m_emailAddr.compare(pStu->getEmail()) == 0;}
    
    const std::string& getFirstName() {return m_firstName;}
    void setFirstName(const std::string& str) {m_firstName = str;}
    
    const std::string& getLastName() {return m_lastName;}
    void setLastName(const std::string& str) {m_lastName = str;}
    
    const std::string& getStudentID() {return m_studentID;}
    void setStudentID(const std::string& str) {m_studentID = str;}
    
    const std::string& getEmail() {return m_emailAddr;}
    void setEmail(const std::string& str) {m_emailAddr = str;}
    
    StudentType getType() {return m_type;}
    
protected:
    Student(std::string *data);
    
    StudentType m_type;
    
private:
    std::string m_firstName, m_lastName;
    std::string m_studentID;
    std::string m_emailAddr;
    
    Queue<TaApplication*> *m_pApplications;
};

#endif //STUDENT_H

