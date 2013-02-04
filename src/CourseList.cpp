#include "CourseList.h"
#include "CuTAES.h"
#include "WindowUtil.h"
#include "Button.h"
#include "StartScreen.h"
#include "StudentInfoScreen.h"
#include "CourseInfoScreen.h"
#include <iostream>
#include <fstream>

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
        show();
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
        show();
        return true;
    }
    return false;
}

//Event handlers

void CourseList::handleBackPressed() {
    StartScreen::instance()->show();
}

void CourseList::handleCoursePressed() {
    StudentInfoScreen *infoScr = new StudentInfoScreen();
    infoScr->show();
    if (infoScr->isInputAccepted()) {
        CourseInfoScreen *courseInfoScr = new CourseInfoScreen();
        courseInfoScr->show();
        delete courseInfoScr;
    }
    delete infoScr;
    CourseList::instance()->show();
}
