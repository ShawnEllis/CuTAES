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
    
    m_pWindow = newwin(h, w, 0, (CuTAES::DEF_W - w) / 2);
    wrefresh(m_pWindow);
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
    draw();
    waitForInput();
    return m_returnState;
}

void Panel::draw() {
    //Decorate the window
    box(m_pWindow, 0 , 0);
    mvwprintw(m_pWindow, 1, (m_width - m_title.length()) / 2, m_title.data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, m_width - 2);
    
    drawComponents();
    
    wrefresh(m_pWindow);
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

