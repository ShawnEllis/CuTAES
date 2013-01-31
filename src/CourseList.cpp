#include "CourseList.h"
#include "CuTAES.h"
#include "WindowUtil.h"
#include "Button.h"

CourseList* CourseList::m_pInstance = 0;

CourseList* CourseList::instance() {
    if (!m_pInstance) {
        m_pInstance = new CourseList();
    }
    return m_pInstance;
}

CourseList::CourseList() {
    WindowUtil::createTitledWindow(&m_pWindow, "Course List");
    Button *pButton = new Button(this, "Back", CuTAES::DEF_W / 2, CuTAES::DEF_H/2 + 3);
    this->add(pButton);

}

CourseList::~CourseList() {
    delwin(m_pWindow);
}

void CourseList::show() {
    //Show the window
	Panel::show();
    Panel::waitForInput();
}
