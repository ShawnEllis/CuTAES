#include "MenuViewSummary.h"
#include "CuTAES.h"
#include "Label.h"
#include "Table.h"
#include "Queue.h"
#include "TaApplication.h"
#include "Student.h"
#include "Database.h"

MenuViewSummary::MenuViewSummary(const std::string& title, const std::string& course) : Panel(title, 143, 25) {
    setReturnState(STATE_ERROR);

    //Create table
    std::string labels[] = {"Student ID", "First", "Last", "Email", "Major", "Year Standing", "Major GPA", "CGPA"};
    int colWidths[] = {10, 16, 16, 32, 32, 13, 9, 4};
    Table *pTable = new Table(this, 1, 3, 10, 8, colWidths, labels);
    pTable->setEditable(false);
    
    //Insert data
    Queue<TaApplication*>* pApplications = Database::instance()->findQueueForCourse(course);
    if (pApplications != 0) {
        
        Node<TaApplication*>* pCur = pApplications->front();
        std::string *data = 0;
        TaApplication *pApplication = 0;
        Student *pStudent = 0;
        while (pCur != 0) {
            pApplication = pCur->value;
            if (pApplication != 0) {
                pStudent = Database::instance()->getStudent(pApplication->getStudentID());
                if (pStudent != 0) {
                    data = new std::string[8];
                    data[0] = pApplication->getStudentID();
                    data[1] = pStudent->getFirstName();
                    data[2] = pStudent->getLastName();
                    data[3] = pStudent->getEmail();
                    data[4] = pStudent->getMajor();
                    data[5] = pStudent->getYearStanding(); //TODO: convert to string
                    data[6] = pStudent->getMajorGPA();
                    data[7] = pStudent->getCGPA();
                    //TODO: sort by GPA
                    pTable->addRow(data);
                }
            }
            pCur = pCur->m_pNext;
        }
    }
    
    add(pTable);
    
    add(new Label(this, "Enter: Back", 1, getHeight() - 2));
}

MenuViewSummary::~MenuViewSummary() {
    
}

/*
 Handle left and right movement
 */
bool MenuViewSummary::handleKeyPress(int key) {
    if (key == CuTAES::KEY_ENT) {
        hide();
        return true;
    }
    return false;
}