#include "UndergradStudent.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#include "CuTAES.h"
#include "StringUtil.h"

#ifdef DEBUG
extern std::ofstream dout;
#endif //DEBUG

using namespace std;

UndergradStudent::UndergradStudent(string *data) : Student(data) {
    m_type = TYPE_UNDERGRAD;
    m_major = data[4];
    m_yearStanding = atoi(data[5].data());
    m_CGPA = atof(data[6].data());
    m_majorGPA = atof(data[7].data());
}

UndergradStudent::~UndergradStudent() {
    
}

int UndergradStudent::getData(std::string** pData) {
    string* data = new string[8];
    data[0] = getFirstName();
    data[1] = getLastName();
    data[2] = getStudentID();
    data[3] = getEmail();
    data[4] = getMajor();
    data[5] = StringUtil::itos(getYearStanding());
    data[6] = StringUtil::ftos(getCGPA());
    data[7] = StringUtil::ftos(getMajorGPA());
    *pData = data;
    return 8;
}