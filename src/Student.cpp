#include "Student.h"
#include <fstream>
#include <iostream>

#include "CuTAES.h"

#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

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
    string filename = CuTAES::instance()->getDataDirectory() + "student/" + m_stuNum + ".txt";
    ofstream file;
    file.open(filename.data());
#ifdef DEBUG
    dout << "Student: Saving to " << filename << std::endl;
#endif //DEBUG
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
#ifdef DEBUG
    dout << "Student: Saved." << std::endl;
#endif //DEBUG
}
