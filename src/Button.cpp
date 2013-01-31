#include "CuTAES.h"
#include "Button.h"
#include "WindowUtil.h"
#include "Panel.h"

using namespace std;

const int BORDER_SIZE_X = 6;
const int BORDER_SIZE_Y = 2;

Button::Button(Panel *pPanel, const string &txt, int cx, int cy) : Component(pPanel), text(txt) {
    width = text.size() + BORDER_SIZE_X * 2;
    height = 1 + BORDER_SIZE_Y * 2;

    x = cx - (width / 2);
    y = cy - (height / 2);

    m_selected = false;
}

void Button::draw() {
    WindowUtil::fillRect(pPanel->getWindow(), x, y, width, height, m_selected ? '-' : ' ');
    WindowUtil::drawRect(pPanel->getWindow(), x, y, width, height);
    mvwprintw(pPanel->getWindow(), y + BORDER_SIZE_Y, x + BORDER_SIZE_X, text.data());
}

void Button::registerActionTriggers() {
    pPanel->registerAction(CuTAES::KEY_ENT, this, &Component::doAction);
}

void Button::doAction() {
    if (m_selected && eventHandler != 0) {
        eventHandler();
    }
}
