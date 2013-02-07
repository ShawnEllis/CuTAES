#include "Component.h"
#include "Panel.h"

Component::Component(Panel *pPanel, int x, int y, int w, int h) : m_pPanel(pPanel), m_x(x), m_y(y), m_width(w), m_height(h) {
    m_selectable = false;
    m_selected = false;
}

Component::~Component() {

}
