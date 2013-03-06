#ifndef MENU_STUDENT_START_H
#define MENU_STUDENT_START_H

#include <curses.h>
#include "Panel.h"

class Button;
class UndergradStudent;
class GradStudent;

class MenuStartStudent : public Panel {
public:
    MenuStartStudent(bool);
    virtual ~MenuStartStudent();
    
    Student* showUndergradDialog(UndergradStudent*);
    Student* showGradDialog(GradStudent*);
    
    bool isUndergrad() {return m_undergrad;}
    
    static void handleBackPressed(Button *pButton);
    static void handleCreatePressed(Button *pButton);
    
private:
    bool m_undergrad;
};

#endif //MENU_STUDENT_START_H

