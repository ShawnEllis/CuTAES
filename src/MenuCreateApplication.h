#ifndef MENU_CREATE_APPLICATION_H
#define MENU_CREATE_APPLICATION_H

#include <curses.h>
#include "Panel.h"

class Table;
class TaApplication;
class MenuWorkExperience;

class MenuCreateApplication : public Panel {
public:
    MenuCreateApplication(const std::string& course, const std::string& studentID, bool=false);
    virtual ~MenuCreateApplication();

    virtual bool getData(TaApplication**);
    
    virtual bool handleKeyPress(int key);
    
private:
    std::string m_strCourse;
    std::string m_strStudentID;
    
    Table *pRelatedCoursesTable, *pTaCoursesTable;
    
    MenuWorkExperience *m_pWorkExperienceMenu;
};

#endif //MENUCREATE_APPLICATION_H

