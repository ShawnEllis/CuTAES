#include "Student.h"
#include <stdio.h>

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
    FILE * pFile;
    string filename = m_stuNum + ".txt";
    pFile = fopen(filename.data(), "w");
    if (pFile != 0) {
        fputs("m_firstName\n", pFile);
        fclose(pFile);
    }
}
