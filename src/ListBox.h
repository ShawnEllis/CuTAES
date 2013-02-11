//
//  ListBox.h
//  Represents a list box component, which contains fields.
//

#ifndef LISTBOX_H
#define LISTBOX_H

#include "Component.h"
#include <form.h>
#include <string>

class Panel;

class ListBox : public Component {
public:
    ListBox(Panel *pPanel, int x=0, int y=0, int w=5, int h=5);
    virtual ~ListBox();
    
    virtual void draw();
    
    virtual bool handleKeyPress(int ch);
    
    void setSelected(bool sel);
    
    int getNumRows() {return m_numRows;}
    
    void addRow(const std::string& str="New Row");
    
private:
    
    void createField(FIELD **pField, int y, std::string str);
    void selectField(int fieldReq);
    
    void createForm();
    
    int m_numRows;
    FORM *m_pForm;
    FIELD **m_pFields;
    
};

#endif //LISTBOX_H
