#include "StartScreen.h"
#include <stdlib.h>

#include "Button.h"
#include "WindowUtil.h"
#include "FormDialog.h"
#include "MenuCourseSelector.h"
#include "MenuCreateApplication.h"
#include "Student.h"
#include "TaApplication.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

StartScreen* StartScreen::m_pInstance = 0;

StartScreen* StartScreen::instance() {
    if (!m_pInstance) {
        m_pInstance = new StartScreen();
    }
    return m_pInstance;
}

StartScreen::StartScreen() : Panel("Student: Select an Action") {
    //Create buttons
    Button *pButton = new Button(this, "Create an Application", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 3);
    pButton->setEventHandler(handleCreatePressed);
    this->add(pButton);

    pButton = new Button(this, "Exit", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 + 3);
    pButton->setEventHandler(handleExitPressed);
    this->add(pButton);
}

StartScreen::~StartScreen() {
}

//Event Handlers

void StartScreen::handleExitPressed(Button *pButton) {
    StartScreen::instance()->hide();
}

void StartScreen::handleCreatePressed(Button *pButton) {
    //Get a course selection from user
    MenuCourseSelector *pCourseSelector = new MenuCourseSelector("Create Application: Select a Course");
    if (pCourseSelector->show() != STATE_SUCCESS) {
        delete pCourseSelector;
        StartScreen::instance()->draw();
        return;
    }
    std::string strCourse = pCourseSelector->getSelectedCourse();
    delete pCourseSelector;
    
    //Create student info dialog
    FormDialog *pForm = new FormDialog("Create " + strCourse + " Application: Enter Student Info", 8);
    pForm->addField("First Name:   ", 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:    ", 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Student ID:   ", 1, 32, FIELDTYPE_INT);
    pForm->addField("Email:        ", 1, 32);
    pForm->addField("Major:        ", 1, 32);
    pForm->addField("Year Standing:", 1, 2, FIELDTYPE_INT);
    int range[] = {0, 1, 12};// TODO: Investigate why float validation is broken on OS X
    pForm->addField("CGPA:", 1, 4, 20, 10, FIELDTYPE_FLOAT, range, 3);
    pForm->addField("Major GPA:", 1, 4, 32, 10, FIELDTYPE_FLOAT, range, 3);
    
    //Show dialog
    if (pForm->show() != STATE_SUCCESS) {
        delete pForm;
        StartScreen::instance()->draw();
        return;
    }
    
    //Get result
    std::string *data = 0;
    if (pForm->getFormData(&data)) {
        //Save student
        Student *stu = new Student(data[0],
                                   data[1],
                                   data[2],
                                   data[3],
                                   data[4],
                                   atoi(data[5].data()),
                                   atof(data[6].data()),
                                   atof(data[7].data()));
        stu->saveToFile();
        delete stu; //TODO: Store student data in a database
    }
    delete pForm;
    
    MenuCreateApplication *pCreateApplication = new MenuCreateApplication(strCourse);
    if (pCreateApplication->show() != STATE_SUCCESS) {
        delete pCreateApplication;
        StartScreen::instance()->draw();
        return;
    }
    
    TaApplication *pApplication = 0;
    if (pCreateApplication->getData(&pApplication)) {
        //TODO: Save application
        delete pApplication;
    }
    
    delete pCreateApplication;
    
    StartScreen::instance()->draw();
}
