#include "Student.h"

using namespace std;

Student::Student(string &first,
                 string &last,
                 string &num,
                 string &email,
                 string &maj,
                 const int year,
                 const float cgpa,
                 const float gpa) : m_firstName(first),
                                    m_lastName(last),
                                    m_stuNum(num),
                                    m_emailAddr(email),
                                    m_major(maj),
                                    m_yearStanding(year),
                                    m_CGPA(cgpa),
                                    m_majorGPA(gpa)
{

}

