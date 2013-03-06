#ifndef GRAD_STUDENT_H
#define GRAD_STUDENT_H

#include "Student.h"

class GradStudent : public Student {
public:
    GradStudent(std::string *data);
    virtual ~GradStudent();

    virtual int getData(std::string**);
    
    const std::string& getResearchArea() {return m_researchArea;}
    void setResearchArea(const std::string& str) {m_researchArea = str;}

    const std::string& getProgram() {return m_program;}
    void setProgram(const std::string& str) {m_program = str;}
    
    const std::string& getSupervisor() {return m_supervisor;}
    void setSupervisor(const std::string& str) {m_supervisor = str;}
    

    
private:
    std::string m_researchArea, m_program, m_supervisor;

};

#endif //GRAD_STUDENT_H