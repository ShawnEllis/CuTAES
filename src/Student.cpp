#include "Student.h"
#include <stdlib.h>

#include "CuTAES.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Queue.h"
#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

using namespace std;

Student::Student(string *data) {
    m_pApplications = new Queue<TaApplication*>();
    m_firstName = data[0];
    m_lastName = data[1];
    m_studentID = data[2];
    m_emailAddr = data[3];
}

void Student::addApplication(TaApplication *pApp) {
    m_pApplications->pushBack(pApp);
}

void Student::setApplications(Queue<TaApplication*>* pApplications) {
    if (pApplications == 0) {
        return;
    }
    if (m_pApplications != 0) {
        delete m_pApplications;
    }
    m_pApplications = new Queue<TaApplication*>(*pApplications);
}

bool Student::hasAppliedForCourse(const std::string &course) {
    if (m_pApplications == 0 || m_pApplications->isEmpty()) {
        return 0;
    }
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
    if (m_studentID.compare("") == 0) {
        return;
    }
    string filename = CuTAES::instance()->getDataDirectory() + "student/" + m_studentID + ".txt";
    ofstream file;
    file.open(filename.data(), fstream::out | fstream::trunc);
#ifdef DEBUG
    dout << "Student: Saving to " << filename << std::endl;
#endif //DEBUG
    if (file.is_open()) {
        file << (m_type == TYPE_UNDERGRAD ? "undergrad" : "grad") << endl;
        string* pData = 0;
        for (int i = 0 ; i < getData(&pData); i++) {
            file << pData[i] << endl;
        }
        delete [] pData;
        file.close();
    }
#ifdef DEBUG
    dout << "Student: Saved." << std::endl;
#endif //DEBUG
}