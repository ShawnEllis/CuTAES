#include "MenuCourseSelector.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "Button.h"
#include "MenuCourseSelector.h"

MenuCourseSelector::MenuCourseSelector(const std::string& title) : Panel(title) {
    setReturnState(STATE_ERROR);
    
    //Create back button    
    Button *pButton = new Button(this, "Back", CuTAES::DEF_W / 4, CuTAES::DEF_H/2);
    pButton->setEventHandler(handleBackPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);
    
    //Create buttons for each course in CourseList.txt
    std::ifstream file;
    file.open((CuTAES::instance()->getDataDirectory() + "CourseList.txt").data());
    
    if (file.is_open()) {
        std::string line;
        int n = 0;
        //Read each course and make buttons for each
        while (file.good()) {
            getline(file, line);
            //TODO: Put buttons in scrollable panel
            pButton = new Button(this, line, CuTAES::DEF_W / 2, 5 + 5 * n++);
            pButton->setEventHandler(handleCoursePressed);
            pButton->setUsrPtr(this);
            this->add(pButton);
        }
        file.close();
    }
}

MenuCourseSelector::~MenuCourseSelector() {
}

/*
    Handle left and right movement
*/
bool MenuCourseSelector::handleKeyPress(int key) {
   if (key == KEY_LEFT) {
        //Select back button
        m_pSelNode->data->setSelected(false);
        m_pSelNode = (m_pSelNode->pPrev == 0) ? m_selectableList.last() : m_selectableList.first();
        m_pSelNode->data->setSelected(true);
        draw();
        return true;
    }
    return false;
}

//Event handlers

void MenuCourseSelector::handleBackPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        MenuCourseSelector *pCourseSelector = ((MenuCourseSelector*)usrPtr);
        pCourseSelector->setReturnState(STATE_CANCEL);
        pCourseSelector->hide();
    }
}

void MenuCourseSelector::handleCoursePressed(Button *pButton) {    
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        MenuCourseSelector *pCourseSelector = ((MenuCourseSelector*)usrPtr);
        pCourseSelector->setSelectedCourse(pButton->getText());
        pCourseSelector->setReturnState(STATE_SUCCESS);
        pCourseSelector->hide();
    }
}
