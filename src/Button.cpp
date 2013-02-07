#include "CuTAES.h"
#include "Button.h"
#include "WindowUtil.h"
#include "Panel.h"

using namespace std;

const int BORDER_SIZE_X = 6;
const int BORDER_SIZE_Y = 2;

Button::Button(Panel *pPanel, const string &txt, int cx, int cy) : Component(pPanel), text(txt) {
    setWidth(text.size() + BORDER_SIZE_X * 2);
    setHeight(1 + BORDER_SIZE_Y * 2);

    setX(cx - (getWidth() / 2));
    setY(cy - (getHeight() / 2));

    setSelectable(true);
}

void Button::draw() {
    WindowUtil::fillRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight(), isSelected() ? ACS_BULLET : ' ');
    WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight());
    mvwprintw(m_pPanel->getWindow(), getY() + BORDER_SIZE_Y, getX() + BORDER_SIZE_X, text.data());
}

void Button::registerActionTriggers() {
    m_pPanel->registerAction(CuTAES::KEY_ENT, this, &Component::doAction);
}

void Button::doAction() {
    if (isSelected() && eventHandler != 0) {
        eventHandler();
    }
}
