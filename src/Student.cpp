#include "Student.h"
#include <fstream>
#include <iostream>

#include "CuTAES.h"

using namespace std;

Student::Student(const string &first,
                 const string &last,
                 const string &num,
                 const string &email,
                 const string &maj,
                 int year,
                 float cgpa,
                 float gpa) : m_firstName(first),
                              m_lastName(last),
                              m_stuNum(num),
                              m_emailAddr(email),
                              m_major(maj),
                              m_yearStanding(year),
                              m_CGPA(cgpa),
                              m_majorGPA(gpa)
{
}

void Student::saveToFile() {
    string filename = CuTAES::instance()->getDataDirectory() + m_stuNum + ".txt";
    ofstream file;
    file.open(filename.data());
    if (file.is_open()) {
        file << m_firstName << "\n";
        file << m_lastName << "\n";
        file << m_stuNum << "\n";
        file << m_emailAddr << "\n";
        file << m_major << "\n";
        file << m_yearStanding << "\n";
        file << m_CGPA << "\n";
        file << m_majorGPA << "\n";
        file.close();
    }
}
