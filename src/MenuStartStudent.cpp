#include "MenuStartStudent.h"
#include <stdlib.h>

#include "Button.h"
#include "DialogForm.h"
#include "MenuCourseSelector.h"
#include "MenuCreateApplication.h"
#include "TaApplication.h"
#include "DialogYesNo.h"
#include "Database.h"
#include "Student.h"
#include "UndergradStudent.h"
#include "GradStudent.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

MenuStartStudent::MenuStartStudent(bool undergrad) : Panel("Student: Select an Action"), m_undergrad(undergrad) {
    //Create buttons
    Button *pButton = new Button(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 6, 5);
    pButton->setEventHandler(handleCreatePressed);
    pButton->setUsrPtr(this);
    this->add(pButton);

    pButton = new Button(this, "Edit an Application", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEnabled(false);
    pButton->setUsrPtr(this);
    this->add(pButton);
    
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEventHandler(handleBackPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);
}

MenuStartStudent::~MenuStartStudent() {
}

//Event Handlers

void MenuStartStudent::handleBackPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        MenuStartStudent *pMenu = ((MenuStartStudent*)usrPtr);
        pMenu->hide();
    }
}

void MenuStartStudent::handleCreatePressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    //Get a pointer to the MenuStartStudent instance
    void* usrPtr = pButton->getUsrPtr();
    MenuStartStudent *pMenu;
    if (usrPtr != 0) {
        pMenu = ((MenuStartStudent*)usrPtr);
    }
    
    pMenu->erase();
    
    //Get the active student
    Student *pActiveStudent = CuTAES::instance()->getActiveUser();
    
    bool shouldExit = false;
    while (!shouldExit) {
        
        //Get a course selection from user
        
        MenuCourseSelector *pCourseSelector = new MenuCourseSelector("Create Application: Select a Course");
        if (pCourseSelector->show() != STATE_SUCCESS) {
            delete pCourseSelector;
            break;
        }
        std::string strCourse = pCourseSelector->getSelectedCourse();
        delete pCourseSelector;
        
        //Get student info
        
        Student* pNewStu;
        if (pMenu->isUndergrad()) {
            if (pActiveStudent != 0 && pActiveStudent->getType() == TYPE_UNDERGRAD) {
                pNewStu = pMenu->showUndergradDialog((UndergradStudent*)pActiveStudent);
            } else {
                pNewStu = pMenu->showUndergradDialog(0);
            }
        } else {
            if (pActiveStudent != 0 && pActiveStudent->getType() == TYPE_GRAD) {
                pNewStu = pMenu->showGradDialog((GradStudent*)pActiveStudent);
            } else {
                pNewStu = pMenu->showGradDialog(0);
            }
        }

        
        if (pNewStu == 0) {
            break;
        }
        //If no active student or fields have been modified
        if (pActiveStudent == 0 || !pActiveStudent->equals(pNewStu)) {
            //Find a student with the given id in the database
            Student *pSavedStudent = Database::instance()->getStudent(pNewStu->getStudentID());
            if (pSavedStudent != 0) {
                //Student exists with that ID. Edit it?
                DialogYesNo *pDialog = new DialogYesNo("Update student " + pNewStu->getStudentID() + "?", DIALOG_YESNO);
                StateType state = pDialog->show();
                delete pDialog;
                if (state != STATE_SUCCESS) {
                    break;                        
                }
                Database::instance()->replaceStudent(pSavedStudent, pNewStu);
                CuTAES::instance()->setActiveUser(pNewStu);
                pActiveStudent = pNewStu;
            } else {
                Database::instance()->addStudent(pNewStu);
                CuTAES::instance()->setActiveUser(pNewStu);
                pActiveStudent = pNewStu;
            }
        }
        
        //Get application info
        
        MenuCreateApplication *pCreateApplication = new MenuCreateApplication(strCourse, pActiveStudent->getStudentID());
        if (pCreateApplication->show() != STATE_SUCCESS) {
            delete pCreateApplication;
            break;
        }
        
        TaApplication *pApplication = 0;
        if (pCreateApplication->getData(&pApplication)) {
            Database::instance()->addApplication(pApplication);
        }
        
        delete pCreateApplication;
        
        DialogYesNo *pDia = new DialogYesNo("Apply for another course?");
        if (pDia->show() != STATE_SUCCESS) {
            shouldExit = true;
        }
        delete pDia;
    }
    pMenu->draw();
}

Student* MenuStartStudent::showUndergradDialog(UndergradStudent *pActiveStudent) {
    //Create student info dialog
    DialogForm *pForm = new DialogForm("Enter Student Info", 8);    
    pForm->addField("First Name:   ", pActiveStudent == 0 ? "" : pActiveStudent->getFirstName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:    ", pActiveStudent == 0 ? "" : pActiveStudent->getLastName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Student ID:   ", pActiveStudent == 0 ? "" : pActiveStudent->getStudentID(), 1, 32, FIELDTYPE_INT);
    pForm->addField("Email:        ", pActiveStudent == 0 ? "" : pActiveStudent->getEmail(), 1, 32);
    pForm->addField("Major:        ", pActiveStudent == 0 ? "" : pActiveStudent->getMajor(), 1, 32);
    pForm->addField("Year Standing:", pActiveStudent == 0 ? "" : StringUtil::itos(pActiveStudent->getYearStanding()), 1, 2, FIELDTYPE_INT);
    int range[] = {0, 1, 12};// TODO: Investigate why float validation is broken on OS X
    pForm->addField("CGPA:", pActiveStudent == 0 ? "" : StringUtil::ftos(pActiveStudent->getCGPA()), 1, 4, 20, 10, FIELDTYPE_FLOAT, range, 3);
    pForm->addField("Major GPA:", pActiveStudent == 0 ? "" : StringUtil::ftos(pActiveStudent->getMajorGPA()), 1, 4, 32, 10, FIELDTYPE_FLOAT, range, 3);
    
    //Show student dialog
    if (pForm->show() != STATE_SUCCESS) {
        return 0;
    }
    
    //Get result
    std::string *data = 0;
    bool isDataValid = pForm->getFormData(&data);
    delete pForm;
    if (isDataValid) {
        return new UndergradStudent(data);
    }
    return 0;
}

Student* MenuStartStudent::showGradDialog(GradStudent *pActiveStudent) {
    //Create student info dialog
    DialogForm *pForm = new DialogForm("Enter Student Info", 7);    
    pForm->addField("First Name:   ", pActiveStudent == 0 ? "" : pActiveStudent->getFirstName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:    ", pActiveStudent == 0 ? "" : pActiveStudent->getLastName(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Student ID:   ", pActiveStudent == 0 ? "" : pActiveStudent->getStudentID(), 1, 32, FIELDTYPE_INT);
    pForm->addField("Email:        ", pActiveStudent == 0 ? "" : pActiveStudent->getEmail(), 1, 32);
    pForm->addField("Research Area:", pActiveStudent == 0 ? "" : pActiveStudent->getResearchArea(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Supervisor:   ", pActiveStudent == 0 ? "" : pActiveStudent->getSupervisor(), 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Program:      ", pActiveStudent == 0 ? "" : pActiveStudent->getProgram(), 1, 4, FIELDTYPE_ALPHA);
    
    //Show student dialog
    if (pForm->show() != STATE_SUCCESS) {
        return 0;
    }
    
    //Get result
    std::string *data = 0;
    bool isDataValid = pForm->getFormData(&data);
    delete pForm;
    if (isDataValid) {
        return new GradStudent(data);
    }
    return 0;
}
