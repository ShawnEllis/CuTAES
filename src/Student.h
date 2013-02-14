#ifndef STUDENT_H
#define STUDENT_H

#include <string>

template<class T>
class Queue;
class TaApplication;

class Student {
public:
	Student(const std::string &first, const std::string &last,
            const std::string &num, const std::string &email,
            const std::string &maj, int year,
            float cgpa, float gpa);

    virtual ~Student() {};

	void saveToFile();
    
    void addApplication(TaApplication *pApp);
    bool hasAppliedForCourse(const std::string& course);

private:
    std::string m_firstName, m_lastName;
    std::string m_stuNum;
    std::string m_emailAddr;
    std::string m_major;
    int m_yearStanding;
    float m_CGPA, m_majorGPA;
    
    Queue<TaApplication*> *m_pApplications;
};

#endif //STUDENT_H

