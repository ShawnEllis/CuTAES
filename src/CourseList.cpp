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
}

CourseList::~CourseList() {
    delwin(m_pWindow);
}

void CourseList::show() {
    //Show the window
	wrefresh(m_pWindow);

    getch();
}
