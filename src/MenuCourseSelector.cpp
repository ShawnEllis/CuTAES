#include "MenuCourseSelector.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "Button.h"
#include "Label.h"
#include "MenuCourseSelector.h"
#include "Database.h"

MenuCourseSelector::MenuCourseSelector(const std::string& title, bool allowAll) : Panel(title), m_allowAll(allowAll) {
    setReturnState(STATE_ERROR);
    
    //Create back button    
    Button *pButton = new Button(this, "Back", CuTAES::DEF_W / 4, CuTAES::DEF_H/2);
    pButton->setEventHandler(handleBackPressed);
    pButton->setUsrPtr(this);
    this->add(pButton);

    //Create buttons for each course in CourseList.txt
    const std::string* courses;
    int count;
    Database::instance()->getCourses(&courses, count);
    for (int i = 0; i < count; i++) {
        //TODO: Put buttons in scrollable panel
        pButton = new Button(this, courses[i], CuTAES::DEF_W / 2, 5 + 5 * i);
        pButton->setEventHandler(handleCoursePressed);
        pButton->setUsrPtr(this);
        this->add(pButton);
    }
    
    if (allowAll) {
        add(new Label(this, "F2: All", getWidth() - 9, getHeight() - 2));
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
        m_pSelNode = (m_pSelNode->pPrev == 0) ? m_pSelectableList->last() : m_pSelectableList->first();
        m_pSelNode->data->setSelected(true);
        draw();
        return true;
    } else if (key == KEY_F(2)) {
        if (m_allowAll) {
            setSelectedCourse("All Courses");
            setReturnState(STATE_SUCCESS);
            hide();
        }
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
