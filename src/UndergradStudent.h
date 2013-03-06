#ifndef UNDERGRAD_STUDENT_H
#define UNDERGRAD_STUDENT_H

#include "Student.h"

class UndergradStudent : public Student {
public:
    UndergradStudent(std::string *data);
    virtual ~UndergradStudent();

    virtual int getData(std::string**);
    
    const std::string& getMajor() {return m_major;}
    void setMajor(const std::string& str) {m_major = str;}
    
    int getYearStanding() {return m_yearStanding;}
    void setYearStanding(int i) {m_yearStanding = i;}
    
    float getCGPA() {return m_CGPA;}
    void setCGPA(float f) {m_CGPA = f;}
    
    float getMajorGPA() {return m_majorGPA;}
    void setMajorGPA(float f) {m_majorGPA = f;}
    
private:
    std::string m_major;
    int m_yearStanding;
    float m_CGPA, m_majorGPA;
};

#endif //UNDERGRAD_STUDENT_H