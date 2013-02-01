#ifndef COURSE_INFO_SCREEN_H
#define COURSE_INFO_SCREEN_H

#include <curses.h>
#include <form.h>
#include "Panel.h"

class CourseInfoScreen : public Panel {
public:
    CourseInfoScreen();
    virtual ~CourseInfoScreen();

    virtual void show();
    virtual void waitForInput();

private:
    FIELD *infoFields[5];
    FORM *pForm;
};

#endif //COURSE_INFO_SCREEN_H

