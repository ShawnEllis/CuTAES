#ifndef DIALOG_LIST_SELECTOR_H
#define DIALOG_LIST_SELECTOR_H

#include "Panel.h"
#include <curses.h>
#include <string>

class Button;

class DialogListSelector : public Panel {
public:
    DialogListSelector(const std::string& title, const std::string* values, int count, bool allowAll=false);
    virtual ~DialogListSelector();

    virtual bool handleKeyPress(int key);

    std::string getSelectedValue() {return m_selectedValue;}
    
    //Event handlers
    static void handleSelection(Button *pButton);
    
private:
    std::string m_selectedValue;
    
    bool m_allowAll;
    
    void scrollScreen();
    void setSelectedValue(const std::string& str) {m_selectedValue = str;}
};

#endif //DIALOG_LIST_SELECTOR_H

