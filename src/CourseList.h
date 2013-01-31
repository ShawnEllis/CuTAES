#ifndef COURSE_LIST_H
#define COURSE_LIST_H

#include <curses.h>
#include "Panel.h"

class CourseList : public Panel {
public:
    static CourseList* instance();
    virtual ~CourseList();

    void show();

private:
    CourseList();
    static CourseList* m_pInstance;
};

#endif //COURSE_LIST_H

