#ifndef COURSE_LIST_H
#define COURSE_LIST_H

#include <curses.h>
#include "Panel.h"

class CourseList : public Panel {
public:
    static CourseList* instance();
    virtual ~CourseList();

    virtual bool handleKeyPress(int key);

    static void handleBackPressed();
    static void handleCoursePressed();

private:
    CourseList();
    static CourseList* m_pInstance;
};

#endif //COURSE_LIST_H

