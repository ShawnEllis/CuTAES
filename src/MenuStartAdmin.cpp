#include "MenuStartAdmin.h"
#include "Button.h"
#include "DialogListSelector.h"
#include "MenuViewSummary.h"
#include "Database.h"

MenuStartAdmin* MenuStartAdmin::m_pInstance = 0;

MenuStartAdmin* MenuStartAdmin::instance() {
    if (!m_pInstance) {
        m_pInstance = new MenuStartAdmin();
    }
    return m_pInstance;
}

MenuStartAdmin::MenuStartAdmin() : Panel("Administrator: Select an Action") {
    //Create buttons
    
    Button *pButton = new Button(this, "View an Application ", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 7, 10, 1);
    pButton->setEnabled(false);
    this->add(pButton);
    
    pButton = new Button(this, "View Summary of Pending Applications",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEventHandler(handleSummaryPressed);
    this->add(pButton);
    
    pButton = new Button(this, "View Summary of Assigned Applicants ",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 2, 1);
    pButton->setEnabled(false);
    this->add(pButton);
    
    pButton = new Button(this, "View and Edit a TA Evaluation ",
                         CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 5, 1);
    pButton->setEnabled(false);
    this->add(pButton);
    
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, pButton->getY() + pButton->getHeight() + 2, 4, 1);
    pButton->setEventHandler(handleBackPressed);
    this->add(pButton);
}

MenuStartAdmin::~MenuStartAdmin() {
}

//Event Handlers

void MenuStartAdmin::handleBackPressed(Button *pButton) {
    MenuStartAdmin::instance()->hide();
}

void MenuStartAdmin::handleSummaryPressed(Button *pButton) {
    //Get a course selection from user
    MenuStartAdmin::instance()->erase();

    //Get course list
    const std::string* courses;
    int count;
    Database::instance()->getCourses(&courses, count);
    //Show dialog
    DialogListSelector *pCourseSelector = new DialogListSelector("View Summary: Select a Course", courses, count, true);
    if (pCourseSelector->show() != STATE_SUCCESS) {
        delete pCourseSelector;
        MenuStartAdmin::instance()->draw();
        return;
    }
    std::string strCourse = pCourseSelector->getSelectedValue();
    if (strCourse.compare("all") == 0) {
        strCourse = "All Courses";
    }
    delete pCourseSelector;
    
    //Show list of applications
    MenuViewSummary *pViewSummary = new MenuViewSummary(strCourse + ": Pending Applications", strCourse);
    pViewSummary->show();
    delete pViewSummary;
    MenuStartAdmin::instance()->draw();
}
