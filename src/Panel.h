#ifndef PANEL_H
#define PANEL_H

#include <curses.h>
#include "List.h"
#include <string>
#include "CuTAES.h"

template <class T>
class ListNode;
class Component;
class ActionTrigger;

class Panel {
public:
    Panel(const std::string &t, int w=CuTAES::DEF_W, int h=CuTAES::DEF_H);
    virtual ~Panel();

    virtual void show();

    virtual void waitForInput();

    /*
        Used by subclasses to handle special input.
        Return true to consume the event.
    */
    virtual bool handleKeyPress(int key) {return false;} 

    void add(Component *c);

    void registerAction(int t, Component *c, void(Component::*a)());

    WINDOW* getWindow() {return m_pWindow;}

protected:
    WINDOW *m_pWindow;
    std::string m_title;
    
    List<Component*> m_componentList; //TODO: Make private
    ListNode<Component*> *pSelNode; //TODO: Clean up menu navigation
    Component* pSelComponent;
    
    virtual void drawComponents();
    
private:    
    List<ActionTrigger*> m_actionTriggerList;
    
};

#endif //PANEL_H

