#include "DialogListSelector.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>

#include "Button.h"
#include "Label.h"
#include "Database.h"

DialogListSelector::DialogListSelector(const std::string& title, const std::string* values, int count, bool allowAll) : Panel(title, 42, 1000), m_allowAll(allowAll) {
    setReturnState(STATE_ERROR);
    
    //Create buttons for each research area
    Button *pButton;
    for (int i = 0; i < count; i++) {
        //TODO: Put buttons in scrollable panel
        pButton = new Button(this, values[i], getWidth() / 2, 4 + 3*i, 6, 1);
        pButton->setEventHandler(handleSelection);
        pButton->setUsrPtr(this);
        this->add(pButton);
    }
    setHeight(pButton->getY() + pButton->getHeight() + 1);
    drawComponents();
 
    int termWidth, termHeight;
    getmaxyx(stdscr, termHeight, termWidth);
    add(new Label(this, "F3: Cancel", 0, termHeight - 1, true));
    if (m_allowAll) {
        add(new Label(this, "F2: All", termWidth - 16, termHeight - 1, true));
    }
}

DialogListSelector::~DialogListSelector() {
}

bool DialogListSelector::handleKeyPress(int key) {
    if (key == KEY_F(3)) {
        setReturnState(STATE_CANCEL);
        hide();
    } else if (key == KEY_F(2)) {
        if (m_allowAll) {
            setSelectedValue("all");
            setReturnState(STATE_SUCCESS);
            hide();
        }
    } else if (key == KEY_UP) {
        //Override panel's default navigation
        //Select prev item
        m_pSelNode->data->setSelected(false);
        m_pSelNode = (m_pSelNode->pPrev != 0) ? m_pSelNode->pPrev : m_pSelectableList->last();
        m_pSelNode->data->setSelected(true);
        scrollScreen();
        draw();
        return true;
    } else if (key == KEY_DOWN) {
        //Override panel's default navigation
        //Select next item
        m_pSelNode->data->setSelected(false);
        m_pSelNode = (m_pSelNode->pNext != 0) ? m_pSelNode->pNext : m_pSelectableList->first();
        m_pSelNode->data->setSelected(true);
        scrollScreen();
        draw();
        return true;
    }

    return false;
}

void DialogListSelector::scrollScreen() {
    //Get term width and height
    int termWidth, termHeight;
    getmaxyx(stdscr, termHeight, termWidth);
    
    //Determine if screen needs to scroll
    int compY = m_pSelNode->data->getY() + m_pSelNode->data->getHeight() + 3;
    if (getScrollY() > m_pSelNode->data->getY()) { 
        //Scroll up
        setScrollY(m_pSelNode->data->getY() + 3 - termHeight);
    } else if (getScrollY() + termHeight < compY) { 
        //Scroll down
        setScrollY(std::min(getHeight() - termHeight, m_pSelNode->data->getY() + m_pSelNode->data->getHeight() + 4 - termHeight));
    }   
}

//Event handlers

void DialogListSelector::handleSelection(Button *pButton) {    
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        DialogListSelector *pDialog = ((DialogListSelector*)usrPtr);
        pDialog->setSelectedValue(pButton->getText());
        pDialog->setReturnState(STATE_SUCCESS);
        pDialog->hide();
    }
}
