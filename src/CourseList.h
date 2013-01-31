#ifndef COURSE_LIST_H
#define COURSE_LIST_H
#include <curses.h>

class Button;

class CourseList {
public:
    static CourseList* instance();
    virtual ~CourseList();

    void show();

private:
    CourseList();
    static CourseList* m_pInstance;
    
    WINDOW *m_pWindow;
};

#endif //COURSE_LIST_H

