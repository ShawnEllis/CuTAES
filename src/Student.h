#ifndef STUDENT_H
#define STUDENT_H

#include <string>

template<class T>
class Queue;
class TaApplication;

class Student {
public:
	Student(const std::string &first, const std::string &last,
            const std::string &id, const std::string &email,
            const std::string &maj, int year,
            float cgpa, float gpa);

    virtual ~Student() {};

	void saveToFile();
    
    void addApplication(TaApplication *pApp);
    bool hasAppliedForCourse(const std::string& course);
    
    const std::string& getFirstName() {return m_firstName;}
    const std::string& getLastName() {return m_lastName;}
    const std::string& getStudentID() {return m_studentID;}
    const std::string& getEmail() {return m_emailAddr;}
    const std::string& getMajor() {return m_major;}
    int getYearStanding() {return m_yearStanding;}
    float getCGPA() {return m_CGPA;}
    float getMajorGPA() {return m_majorGPA;}
    
private:
    std::string m_firstName, m_lastName;
    std::string m_studentID;
    std::string m_emailAddr;
    std::string m_major;
    int m_yearStanding;
    float m_CGPA, m_majorGPA;
    
    Queue<TaApplication*> *m_pApplications;
};

#endif //STUDENT_H

