#include "MenuStartStudent.h"
#include <stdlib.h>

#include "Button.h"
#include "DialogForm.h"
#include "MenuCourseSelector.h"
#include "MenuCreateApplication.h"
#include "Student.h"
#include "TaApplication.h"
#include "DialogYesNo.h"
#include "Database.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

MenuStartStudent* MenuStartStudent::m_pInstance = 0;

MenuStartStudent* MenuStartStudent::instance() {
    if (!m_pInstance) {
        m_pInstance = new MenuStartStudent();
    }
    return m_pInstance;
}

MenuStartStudent::MenuStartStudent() : Panel("Student: Select an Action") {
    //Create buttons
    Button *pButton = new Button(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 6, 5);
    pButton->setEventHandler(handleCreatePressed);
    this->add(pButton);

    pButton = new Button(this, "Edit an Application", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEnabled(false);
    this->add(pButton);
    
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 3);
    pButton->setEventHandler(handleBackPressed);
    this->add(pButton);
}

MenuStartStudent::~MenuStartStudent() {
}

//Event Handlers

void MenuStartStudent::handleBackPressed(Button *pButton) {
    MenuStartStudent::instance()->hide();
}

void MenuStartStudent::handleCreatePressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    
    MenuStartStudent::instance()->erase();
    
    Student *pStudent = CuTAES::instance()->getActiveUser();
    if (pStudent == 0) {
        MenuStartStudent::instance()->draw();
        return;
    }
    
    bool shouldExit = false;
    while (!shouldExit) {    
        //Get a course selection from user
        MenuCourseSelector *pCourseSelector = new MenuCourseSelector("Create Application: Select a Course");
        if (pCourseSelector->show() != STATE_SUCCESS) {
            delete pCourseSelector;
            MenuStartStudent::instance()->draw();
            return;
        }
        std::string strCourse = pCourseSelector->getSelectedCourse();
        delete pCourseSelector;
        
        MenuCreateApplication *pCreateApplication = new MenuCreateApplication(strCourse, pStudent->getStudentID());
        if (pCreateApplication->show() != STATE_SUCCESS) {
            delete pCreateApplication;
            MenuStartStudent::instance()->draw();
            return;
        }
        
        TaApplication *pApplication = 0;
        if (pCreateApplication->getData(&pApplication)) {
            pApplication->saveToFile();
            Database::instance()->addApplication(pApplication);
        }
        
        delete pCreateApplication;
        
        DialogYesNo *pDia = new DialogYesNo("Apply for another course?");
        if (pDia->show() != STATE_SUCCESS) {
            shouldExit = true;
        }
        delete pDia;
    }
    MenuStartStudent::instance()->draw();
}
