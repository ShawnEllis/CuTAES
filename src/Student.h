
#ifndef STUDENT_H_
#define STUDENT_H_

#include <string>

class Student {
public:
	Student(const std::string &first, const std::string &last,
            const std::string &num, const std::string &email,
            const std::string &maj, int year,
            float cgpa, float gpa);

    virtual ~Student() {};

	void saveToFile();

private:
    std::string m_firstName, m_lastName;
    std::string m_stuNum;
    std::string m_emailAddr;
    std::string m_major;
    int m_yearStanding;
    float m_CGPA, m_majorGPA;
};

#endif //STUDENT_H

