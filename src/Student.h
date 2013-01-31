
#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>

class Student {
public:
	Student(std::string &first, std::string &last,
            std::string &num, std::string &email,
            std::string &maj, const int year,
            const float cgpa, const float gpa);

    virtual ~Student() {};
	
private:
    std::string &m_firstName, &m_lastName;
    std::string &m_stuNum;
    std::string &m_emailAddr;
    std::string &m_major;
    int m_yearStanding;
    float m_CGPA, m_majorGPA;
};

#endif //STUDENT_H

