#ifndef MENU_COURSE_SELECTOR_H
#define MENU_COURSE_SELECTOR_H

#include "Panel.h"
#include <curses.h>
#include <string>

class Button;

class MenuCourseSelector : public Panel {
public:
    MenuCourseSelector(const std::string& title, bool allowAll=false);
    virtual ~MenuCourseSelector();

    virtual bool handleKeyPress(int key);

    std::string getSelectedCourse() {return m_selectedCourse;}
    
    //Event handlers
    static void handleBackPressed(Button *pButton);
    static void handleCoursePressed(Button *pButton);
    
private:
    std::string m_selectedCourse;
    
    bool m_allowAll;
    
    void setSelectedCourse(const std::string& str) {m_selectedCourse = str;}
};

#endif //MENU_COURSE_SELECTOR_H

