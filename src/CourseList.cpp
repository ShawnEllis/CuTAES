#include "CourseList.h"
#include "CuTAES.h"
#include "WindowUtil.h"
#include "Button.h"
#include "StartScreen.h"
#include "StudentInfoScreen.h"

CourseList* CourseList::m_pInstance = 0;

CourseList* CourseList::instance() {
    if (!m_pInstance) {
        m_pInstance = new CourseList();
    }
    return m_pInstance;
}

CourseList::CourseList() : Panel("Course List") {
    Button *pButton = new Button(this, "Course", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 - 3);
    pButton->setEventHandler(handleCoursePressed);
    this->add(pButton);
    pButton = new Button(this, "Back", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 + 3);
    pButton->setEventHandler(handleBackPressed);
    this->add(pButton);
}

CourseList::~CourseList() {
}

void CourseList::draw() {
}

//Event handlers

void CourseList::handleBackPressed() {
    StartScreen::instance()->show();
}

void CourseList::handleCoursePressed() {
    StudentInfoScreen *infoScr = new StudentInfoScreen();
    infoScr->show();
    delete infoScr;
}
