#ifndef MENU_CREATE_APPLICATION_H
#define MENU_CREATE_APPLICATION_H

#include <curses.h>
#include "Panel.h"

class Table;
class TaApplication;

class MenuCreateApplication : public Panel {
public:
    MenuCreateApplication(const std::string& course, const std::string& studentID);
    virtual ~MenuCreateApplication();

    virtual bool getData(TaApplication**);
    
    virtual bool handleKeyPress(int key);
    
private:
    std::string m_strCourse;
    std::string m_strStudentID;
    
    Table *pRelatedCoursesTable, *pTaCoursesTable;
    
};

#endif //MENUCREATE_APPLICATION_H

