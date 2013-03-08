#include "Panel.h"
#include <stdio.h>

#include "Component.h"
#include "ListNode.h"
#include "WindowUtil.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

using namespace std;

Panel::Panel(const string &t, int w, int h) : m_title(t), m_width(w), m_height(h) {
    m_pSelNode = 0;
    m_returnState = STATE_VOID;
    
    m_scrollX = 0;
    m_scrollY = 0;
    
    getmaxyx(stdscr, m_termHeight, m_termWidth); //Used to center panel
    updateScreenCoords();
    
    m_pComponentList = new List<Component*>();
    m_pSelectableList = new List<Component*>();
    
    m_pWindow = newpad(h, w);

    m_pPanel = new_panel(m_pWindow);
}

Panel::~Panel() {
//    delwin(m_pWindow);
    m_pComponentList->clear<Component*>();
    delete m_pComponentList;
    delete m_pSelectableList;
}

StateType Panel::show() {
#ifdef DEBUG
    dout << "Show panel " << m_title << endl;
#endif //DEBUG
    m_visible = true;
    show_panel(m_pPanel);
    draw();
    waitForInput();
    return m_returnState;
}

void Panel::hide()  {
    hide_panel(m_pPanel);
    m_visible = false;
    WindowUtil::fillRect(m_pWindow, 0, 0, getWidth(), getHeight(), ' ');
    wrefresh(m_pWindow);
    prefresh(m_pWindow, m_scrollY, m_scrollX, m_screenYi, m_screenXi, m_screenYf, m_screenXf);
    update_panels();
}

void Panel::draw() {
    //Stay centered on screen
    int termW, termH;
    getmaxyx(stdscr, termH, termW);
    if (termW != m_termWidth || termH != m_termHeight) {
        adjustSize();
    }
    
    //Decorate the window
    WindowUtil::drawRect(m_pWindow, 0, 0, getWidth(), getHeight());
    mvwprintw(m_pWindow, 1, (getWidth() - m_title.length()) / 2, m_title.data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, getWidth() - 2);
    
    drawComponents();
    
    refresh();
    wrefresh(m_pWindow);
    prefresh(m_pWindow, m_scrollY, m_scrollX, m_screenYi, m_screenXi, m_screenYf, m_screenXf);
    update_panels();
}

/*
 * Iterate through componentList and draw components
 */
void Panel::drawComponents() {
    ListNode<Component*>* cur = m_pComponentList->first();
    while (cur != 0) {
        cur->data->draw();
        cur = cur->pNext;
    }
}

void Panel::adjustSize() {
    getmaxyx(stdscr, m_termHeight, m_termWidth);
    clear();
    updateScreenCoords();
    wresize(m_pWindow, getHeight(), getWidth()); //Necessary because resizing term resizes term-sized windows
    move_panel(m_pPanel, (m_termHeight - getHeight()) / 2, (m_termWidth - getWidth()) / 2);
}

void Panel::updateScreenCoords() {
    m_screenXi = std::max((m_termWidth - m_width) / 2, 0);
    m_screenYi = std::max((m_termHeight - m_height) / 2, 0);
    m_screenXf = std::min(m_screenXi + m_width, m_termWidth - 1);
    m_screenYf = std::min(m_screenYi + m_height, m_termHeight - 1);
}

void Panel::waitForInput() {
    while (m_visible) {
        update_panels();
        wrefresh(m_pWindow);
        prefresh(m_pWindow, m_scrollY, m_scrollX, m_screenYi, m_screenXi, m_screenYf, m_screenXf);
        int ch = getch();
        if (m_pSelNode != 0 && m_pSelNode->data->handleKeyPress(ch)) {
            continue;
        }
        if (handleKeyPress(ch)) {
            continue;
        }
        if (m_pSelectableList->getSize() > 1) {
            if (ch == KEY_UP || ch == KEY_LEFT) {
                //Select prev item
                m_pSelNode->data->setSelected(false);
                m_pSelNode = (m_pSelNode->pPrev != 0) ? m_pSelNode->pPrev : m_pSelectableList->last();
                m_pSelNode->data->setSelected(true);
                draw();
            } else if (ch == KEY_DOWN || ch == KEY_RIGHT) {
                //Select next item
                m_pSelNode->data->setSelected(false);
                m_pSelNode = (m_pSelNode->pNext != 0) ? m_pSelNode->pNext : m_pSelectableList->first();
                m_pSelNode->data->setSelected(true);
                draw();
            }
        }
    }
}

void Panel::add(Component *c) {
    m_pComponentList->addBack(c);
    if (c->isSelectable()) {
        m_pSelectableList->addBack(c);
        if (m_pSelNode == 0) {
            m_pSelNode = m_pSelectableList->first();
            m_pSelNode->data->setSelected(true);
        }
    }
}

