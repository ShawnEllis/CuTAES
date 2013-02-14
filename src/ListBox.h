//
//  ListBox.h
//  Represents a list box component, which contains fields.
//

#ifndef LISTBOX_H
#define LISTBOX_H

#include "Component.h"
#include <form.h>
#include <string>
#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG
class Panel;

class ListBox : public Component {
public:
    ListBox(Panel *pPanel, int x=0, int y=0, int w=5, int h=5);
    virtual ~ListBox();
    
    virtual void draw();
    
    virtual bool handleKeyPress(int ch);
    
    std::string getDataAt(int r);
    void getData(std::string**);
    
    int getNumRows() {return m_numRows;}
    void addRow(const std::string& str="New Row");
    
    int getCurRow() {return m_curRow;}
    void setCurRow(int r);
    
    virtual void setSelected(bool sel);
    
private:
    
    void selectField(int formDriverReq);
    
    void createField(FIELD **pField, int y, std::string str);
    void createForm();
    
    int m_numRows;
    int m_curRow;
    FORM *m_pForm;
    FIELD **m_pFields;
    
};

#endif //LISTBOX_H
