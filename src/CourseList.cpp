#include "CourseList.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "Button.h"
#include "StartScreen.h"
#include "FormDialog.h"
#include "CourseInfoScreen.h"
#include "WindowUtil.h"
#include "Student.h"

using namespace std;

CourseList* CourseList::m_pInstance = 0;

CourseList* CourseList::instance() {
    if (!m_pInstance) {
        m_pInstance = new CourseList();
    }
    return m_pInstance;
}

CourseList::CourseList() : Panel("Course List") {
    //Create back button    
    Button *pButton = new Button(this, "Back", CuTAES::DEF_W / 4, CuTAES::DEF_H/2);
    pButton->setEventHandler(handleBackPressed);
    this->add(pButton);
    //Create buttons for each course in CourseList.txt
    ifstream file;
    file.open((CuTAES::instance()->getDataDirectory() + "CourseList.txt").data());
    if (file.is_open()) {
        string line;
        int n = 0;
        while (file.good()) {
            getline(file, line);
            //Create a button
            //TODO: Put buttons in scrollable panel
            pButton = new Button(this, line, CuTAES::DEF_W / 2, 5 + 5 * n++);
            pButton->setEventHandler(handleCoursePressed);
            this->add(pButton);
        }
        file.close();
    }
}

CourseList::~CourseList() {
}

/*
    Handle left and right movement
*/
bool CourseList::handleKeyPress(int key) {
    if (key == KEY_LEFT) {
        //Select back button
        pSelComponent->setSelected(false);
        pSelNode = m_componentList.first();
        pSelComponent = pSelNode->data;
        pSelComponent->setSelected(true);
        draw();
        return true;
    } else if (key == KEY_RIGHT) {
        //Select next item
        pSelComponent->setSelected(false);
        if (pSelNode->pNext != 0) {
            pSelNode = pSelNode->pNext;
        } else {
            pSelNode = m_componentList.first();
        }
        pSelComponent = pSelNode->data;
        pSelComponent->setSelected(true);
        draw();
        return true;
    }
    return false;
}

//Event handlers

void CourseList::handleBackPressed() {
    CourseList::instance()->hide();
//    StartScreen::instance()->show();
}

void CourseList::handleCoursePressed() {
    //Create and show student info dialog
    FormDialog *pForm = new FormDialog("Enter Student Info", 8);
    pForm->addField("First Name:   ", 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Last Name:    ", 1, 32, FIELDTYPE_ALPHA);
    pForm->addField("Student ID:   ", 1, 32, FIELDTYPE_INT);
    pForm->addField("Email:        ", 1, 32);
    pForm->addField("Major:        ", 1, 32);
    pForm->addField("Year Standing:", 1, 2, FIELDTYPE_INT);
//    int range[] = {1, 1, 12}; TODO: Investigate why float validation is broken on OS X
    pForm->addField("CGPA:", 1, 4, 20, 10, FIELDTYPE_FLOAT/*, range, 3*/);
    pForm->addField("Major GPA:", 1, 4, 32, 10, FIELDTYPE_FLOAT/*, range, 3*/);
    pForm->show();
    //Get result
    bool formAccepted;
    string *data;
    pForm->getFormData(&formAccepted, &data);
    delete pForm;
    if (formAccepted) {
        //Save student
        Student *stu = new Student(data[0], data[1], data[2], data[3], data[4],
                                   atoi(data[5].data()), atof(data[6].data()), atof(data[7].data()));
        stu->saveToFile();
        delete stu;

        //Create and show course info dialog
//        pForm = new FormDialog("Enter Course Info", 8);
//        pForm->addField("Relevant Work Experience:|Include responsibilities, duration, start and end dates.", 4, 32, 11, 0);
//        pForm->addList("List test", 10, 10, 0, 0);
//        pForm->show();
        
        CourseInfoScreen *courseInfoScr = new CourseInfoScreen();
        courseInfoScr->show();
        delete courseInfoScr;
    }
    //TODO: Store student data and application data in some global queue
    
    CourseList::instance()->show();
}
