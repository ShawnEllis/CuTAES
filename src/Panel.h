#ifndef PANEL_H
#define PANEL_H

#include <curses.h>
#include <string>
#include <panel.h>

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
    virtual void hide();

    /*
        Used by subclasses to handle special input.
        Return true to consume the event.
    */
    virtual bool handleKeyPress(int key) {return false;} 

    void add(Component *c);

    WINDOW* getWindow() {return m_pWindow;}
    
    int getWidth() {return m_width;}
    int getHeight() {return m_height;}
    int getX() {return getbegx(m_pWindow);}
    int getY() {return getbegy(m_pWindow);}
    
    std::string getTitle() {return m_title;}
    
    StateType getReturnState() {return m_returnState;}
    
protected:
    WINDOW *m_pWindow;
    PANEL *m_pPanel;
    
    List<Component*> *m_pSelectableList;
    ListNode<Component*> *m_pSelNode;
    
    virtual void draw();
    void erase() {hide_panel(m_pPanel); update_panels();}
    
    virtual void drawComponents();
    
    virtual void waitForInput();
    
    void setReturnState(StateType state) {m_returnState = state;}
    
    int getScrollX() {return m_scrollX;}
    void setScrollX(int scrollX) {m_scrollX = scrollX;}
    void scrollX(int scrollX) {m_scrollX += scrollX;}
    
    int getScrollY() {return m_scrollY;}
    void setScrollY(int scrollY) {m_scrollY = scrollY;}
    void scrollY(int scrollY) {m_scrollY += scrollY;}
    
    void setWidth(int w) {m_width = w; adjustSize();}
    void setHeight(int h) {m_height = h; adjustSize();}
    
    void adjustSize();
    
private:
    bool termSizeChanged();
    void updateScreenCoords();
    
    bool m_visible;
    int m_width, m_height;
    int m_termWidth, m_termHeight;
    int m_screenXi, m_screenYi, m_screenXf, m_screenYf;
    int m_scrollY, m_scrollX;

    std::string m_title;
    
    StateType m_returnState;
    
    List<Component*> *m_pComponentList;
};

#endif //PANEL_H

