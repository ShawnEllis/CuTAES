#ifndef STUDENT_INFO_SCREEN_H
#define STUDENT_INFO_SCREEN_H

#include <curses.h>
#include <form.h>
#include "Panel.h"

class StudentInfoScreen : public Panel {
public:
    StudentInfoScreen();
    virtual ~StudentInfoScreen();

    virtual void show();
    virtual void waitForInput();

private:
    FIELD *infoFields[9];
    FORM *pForm;
};

#endif //STUDENT_INFO_SCREEN_H

