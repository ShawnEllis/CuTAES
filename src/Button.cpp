#include "CuTAES.h"
#include "Button.h"
#include "WindowUtil.h"
#include "Panel.h"

using namespace std;

Button::Button(Panel *pPanel, const string &txt, int cx, int cy, int borderX, int borderY)
: Component(pPanel), m_text(txt), m_borderX(borderX), m_borderY(borderY) {
    setWidth(m_text.size() + borderX * 2);
    setHeight(1 + borderY * 2);

    setX(cx - (getWidth() / 2));
    setY(cy - (getHeight() / 2));

    setSelectable(true);
    m_enabled = true;
}

/*
 *  Draw the button.
 *  If selected, use term.bold color.
 *  If enabled, use default color.
 *  If disabled, use dashes.
 */
void Button::draw() {
    
    if (isSelected()) {
        //Use terminal's bold color
        wattron(m_pPanel->getWindow(), A_BOLD);
    }
    
    //Fill the button area
    WindowUtil::fillRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight(), ' ');    
    WindowUtil::fillRect(m_pPanel->getWindow(), getX() - 2, getY(), getWidth() + 4, 1, ' '); //Ensure text is cleared
    
    //Draw the button
    if (m_borderY > 0) {
        if (!m_enabled) {
            WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight(), '-', '-', '-', '-', '-', '-', '-', '-');
        } else {
            WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight());
        }
    }
    
    //Draw button text
    if (isSelected()) {
        //Draw '> <' around text to show selection
        mvwprintw(m_pPanel->getWindow(), getY() + m_borderY, getX() + m_borderX - 2, ("> " + m_text + " <").data());
        wattroff(m_pPanel->getWindow(), A_BOLD);
    } else if (!m_enabled) {
        //Draw text
        wattron(m_pPanel->getWindow(), A_DIM);
        mvwprintw(m_pPanel->getWindow(), getY() + m_borderY, getX() + m_borderX, m_text.data());
        wattroff(m_pPanel->getWindow(), A_DIM);
    } else {
        //Draw text
        mvwprintw(m_pPanel->getWindow(), getY() + m_borderY, getX() + m_borderX, m_text.data());
    }
}

bool Button::handleKeyPress(int ch) {
    if (ch == CuTAES::KEY_ENT) {
        if (eventHandler != 0) {
            //Do action
            eventHandler(this);
            return true;
        }
    }
    return false;
}
