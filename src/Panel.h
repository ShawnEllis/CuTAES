#ifndef PANEL_H
#define PANEL_H

#include <curses.h>
#include <string>

#include "CuTAES.h"
#include "StateType.h"
#include "List.h"

template <class T>
class ListNode;

class Component;

class Panel {
public:
    Panel(const std::string &t, int w=CuTAES::DEF_W, int h=CuTAES::DEF_H);
    virtual ~Panel();

    virtual StateType show();
    virtual void hide() {m_visible = false;}

    /*
        Used by subclasses to handle special input.
        Return true to consume the event.
    */
    virtual bool handleKeyPress(int key) {return false;} 

    void add(Component *c);

    WINDOW* getWindow() {return m_pWindow;}
    
    int getWidth() {return m_width;}
    int getHeight() {return m_height;}
    std::string getTitle() {return m_title;}
    
    StateType getReturnState() {return m_returnState;}
    
protected:
    WINDOW *m_pWindow;
    
    List<Component*> m_selectableList; //TODO: Make private
    ListNode<Component*> *m_pSelNode; //TODO: Clean up menu navigation
    
    virtual void draw();
    virtual void drawComponents();
    
    virtual void waitForInput();
    
    void setReturnState(StateType state) {m_returnState = state;}
    
private:
    bool m_visible;
    int m_width, m_height;

    std::string m_title;
    
    StateType m_returnState;
    
    List<Component*> m_componentList; //TODO: Make private
};

#endif //PANEL_H

