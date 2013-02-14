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
    
    m_componentList = *(new List<Component*>());
    m_selectableList = *(new List<Component*>());
    
    getmaxyx(stdscr, m_termHeight, m_termWidth); //Used to determine when to re-center panel
    
    m_pWindow = newwin(h, w, (m_termHeight - h) / 2, (m_termWidth - w) / 2);

    m_pPanel = new_panel(m_pWindow);
}

Panel::~Panel() {
    delwin(m_pWindow);
    //TODO: Free components
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
    update_panels();
}

void Panel::draw() {
    //Stay centered on screen
    int termW, termH;
    getmaxyx(stdscr, termH, termW);
    if (termW != m_termWidth || termH != m_termHeight) {
        clear();
        m_termWidth = termW;
        m_termHeight = termH;
        wresize(m_pWindow, getHeight(), getWidth()); //Necessary because resizing term resizes term-sized windows
        move_panel(m_pPanel, (m_termHeight - getHeight()) / 2, (m_termWidth - getWidth()) / 2);
    }
    
    //Decorate the window
    WindowUtil::drawRect(m_pWindow, 0, 0, getWidth(), getHeight());
    mvwprintw(m_pWindow, 1, (getWidth() - m_title.length()) / 2, m_title.data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, getWidth() - 2);
    
    drawComponents();
    
    refresh();
    update_panels();
}

/*
 * Iterate through componentList and draw components
 */
void Panel::drawComponents() {
    ListNode<Component*>* cur = m_componentList.first();
    while (cur != 0) {
        cur->data->draw();
        cur = cur->pNext;
    }
}

void Panel::waitForInput() {
    while (m_visible) {
        update_panels();
        wrefresh(m_pWindow);
        int ch = getch();
        if (m_pSelNode != 0 && m_pSelNode->data->handleKeyPress(ch)) {
            continue;
        }
        if (handleKeyPress(ch)) {
            continue;
        }
        if (m_selectableList.getSize() > 1) {
            if (ch == KEY_UP || ch == KEY_LEFT) {
                //Select prev item
                m_pSelNode->data->setSelected(false);
                m_pSelNode = (m_pSelNode->pPrev != 0) ? m_pSelNode->pPrev : m_selectableList.last();
                m_pSelNode->data->setSelected(true);
                draw();
            } else if (ch == KEY_DOWN || ch == KEY_RIGHT) {
                //Select next item
                m_pSelNode->data->setSelected(false);
                m_pSelNode = (m_pSelNode->pNext != 0) ? m_pSelNode->pNext : m_selectableList.first();
                m_pSelNode->data->setSelected(true);
                draw();
            }
        }
    }
}

void Panel::add(Component *c) {
    m_componentList.addBack(c);
    if (c->isSelectable()) {
        m_selectableList.addBack(c);
        if (m_pSelNode == 0) {
            m_pSelNode = m_selectableList.first();
            m_pSelNode->data->setSelected(true);
        }
    }
}

