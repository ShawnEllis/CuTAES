#include "CuTAES.h"
#include "Button.h"
#include "WindowUtil.h"
#include "Panel.h"

using namespace std;

const int BORDER_SIZE_X = 6;
const int BORDER_SIZE_Y = 2;

Button::Button(Panel *pPanel, const string &txt, int cx, int cy) : Component(pPanel), m_text(txt) {
    setWidth(m_text.size() + BORDER_SIZE_X * 2);
    setHeight(1 + BORDER_SIZE_Y * 2);

    setX(cx - (getWidth() / 2));
    setY(cy - (getHeight() / 2));

    setSelectable(true);
}

void Button::draw() {    
    if (isSelected()) {
        //Use terminal's bold color
        wattron(m_pPanel->getWindow(), A_BOLD);
    }
    //Draw the button
    WindowUtil::fillRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight(), ' ');    
    WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight());
    if (isSelected()) {
        //Draw '> <' around text to show selection
        mvwprintw(m_pPanel->getWindow(), getY() + BORDER_SIZE_Y, getX() + BORDER_SIZE_X - 2, ("> " + m_text + " <").data());
        wattroff(m_pPanel->getWindow(), A_BOLD);
    } else {
        mvwprintw(m_pPanel->getWindow(), getY() + BORDER_SIZE_Y, getX() + BORDER_SIZE_X, m_text.data());
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
