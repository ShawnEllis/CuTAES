#include "GradStudent.h"
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

GradStudent::GradStudent(string *data) : Student(data) {
    m_type = TYPE_GRAD;
    m_researchArea = data[4];
    m_supervisor = data[5];
    m_program = data[6];
}

GradStudent::~GradStudent() {
    
}

int GradStudent::getData(std::string** pData) {
    string* data = new string[7];
    data[0] = getFirstName();
    data[1] = getLastName();
    data[2] = getStudentID();
    data[3] = getEmail();
    data[4] = getResearchArea();
    data[5] = getSupervisor();
    data[6] = getProgram();
    *pData = data;
    return 7;
}