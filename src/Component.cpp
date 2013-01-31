#include "Component.h"

Component::Component(WINDOW *win) : pWindow(win) {
    m_selectable = true;
}

Component::~Component() {

}
