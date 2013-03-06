#ifndef MENU_WORK_EXPERIENCE_H
#define MENU_WORK_EXPERIENCE_H

#include <curses.h>
#include "Panel.h"

class Table;
class TaApplication;

class MenuWorkExperience : public Panel {
public:
    MenuWorkExperience(const std::string& title);
    virtual ~MenuWorkExperience();

    virtual bool getData(TaApplication*);
    
    virtual bool handleKeyPress(int key);
    
private:
    Table *m_pWorkExperienceTable;
};

#endif //MENU_WORK_EXPERIENCE_H

