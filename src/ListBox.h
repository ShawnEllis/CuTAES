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
    
    virtual void registerActionTriggers();
    
    virtual void doAction();
    
protected:
    void addRow(const std::string& str="New Row");

private:
    int m_numRows;
    FORM *m_pForm;
    FIELD **m_pFields;
    
    void createField(FIELD **pField, int y, std::string str);
    
};

#endif //LISTBOX_H
