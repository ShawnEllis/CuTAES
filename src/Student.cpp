#include "Student.h"
#include <fstream>
#include <iostream>

#include "CuTAES.h"
#include "Queue.h"
#include "TaApplication.h"
#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

using namespace std;

Student::Student(const string &first,
                 const string &last,
                 const string &id,
                 const string &email,
                 const string &maj,
                 int year,
                 float cgpa,
                 float gpa) : m_firstName(first),
                              m_lastName(last),
                              m_studentID(id),
                              m_emailAddr(email),
                              m_major(maj),
                              m_yearStanding(year),
                              m_CGPA(cgpa),
                              m_majorGPA(gpa)
{
    m_pApplications = new Queue<TaApplication*>();
}

void Student::addApplication(TaApplication *pApp) {
    m_pApplications->pushBack(pApp);
}

bool Student::hasAppliedForCourse(const std::string &course) {
    Node<TaApplication*> *pCur = m_pApplications->front();
    while (pCur != 0) {
        if (pCur->value->getCourse().compare(course) == 0) {
            return true;
        }
        pCur = pCur->m_pNext;
    }
    return false;
}

void Student::saveToFile() {
    string filename = CuTAES::instance()->getDataDirectory() + "student/" + m_studentID + ".txt";
    ofstream file;
    file.open(filename.data());
#ifdef DEBUG
    dout << "Student: Saving to " << filename << std::endl;
#endif //DEBUG
    if (file.is_open()) {
        file << m_firstName << "\n";
        file << m_lastName << "\n";
        file << m_studentID << "\n";
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
