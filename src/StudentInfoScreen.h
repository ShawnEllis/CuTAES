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

    static void handleNextPressed();
    static void handleCancelPressed();

private:
    FIELD *infoFields[8];
    FORM *pForm;
};

#endif //STUDENT_INFO_SCREEN_H

