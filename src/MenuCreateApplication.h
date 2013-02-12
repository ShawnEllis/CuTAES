#ifndef MENUCREATEAPPLICATION_H
#define MENUCREATEAPPLICATION_H

#include <curses.h>
#include "Panel.h"

class Table;
class TaApplication;

class MenuCreateApplication : public Panel {
public:
    MenuCreateApplication(const std::string& strCourse);
    virtual ~MenuCreateApplication();

    virtual bool getData(TaApplication**);
    
    virtual bool handleKeyPress(int key);
    
private:
    const std::string& m_strCourse;
    
    Table *pRelatedCoursesTable, *pTaCoursesTable;
    
};

#endif //MENUCREATEAPPLICATION_H

