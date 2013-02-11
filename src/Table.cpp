#include "Table.h"

#include "Panel.h"
#include "Label.h"
#include "ListBox.h"
#include "WindowUtil.h"

Table::Table(Panel *pPanel, int x, int y, int colH, int numCols, int *colWidths, std::string* labels) : Component(pPanel, x, y, 1, colH + 2), m_numCols(numCols) {
    setSelectable(true);
    m_pLists = new ListBox*[numCols];
    m_curList = 0;
    
    //Set width and init m_colWidths
    m_colWidths = new int[numCols];
    int w = 4;
    for (int i = 0; i < numCols; i++) {
        w += colWidths[i];
        m_colWidths[i] = colWidths[i];
    }
    setWidth(w + numCols);
    
    w = 0;
    for (int i = 0; i < numCols; i++) {
        m_pLists[i] = new ListBox(pPanel, x + w, y + 2, colWidths[i] + 2, colH);
        Label *pLabel = new Label(pPanel, labels[i], x + w + 1, y + 1);
        pPanel->add(pLabel);
        w += m_colWidths[i] + 1;
    }
                    
    m_pLists[0]->setSelected(true);
}

Table::~Table() {
    
}

void Table::draw() {
    WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY() + 2, getWidth() - m_numCols + 1, getHeight() - 2);
    WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth() - m_numCols + 1, getHeight());
    int w = m_colWidths[0] + 1;
    for (int i = 1; i < m_numCols; i++) {
        WindowUtil::drawVLine(m_pPanel->getWindow(), getX() + w, getY() + 1, getHeight() - 2);
        w += m_colWidths[i] + 1;
    }
}

bool Table::handleKeyPress(int key) {
    if (key == KEY_LEFT) {
        m_pLists[m_curList]->setSelected(false);
        m_curList = (m_curList - 1 < 0) ? m_numCols - 1 : m_curList - 1;
        m_pLists[m_curList]->setSelected(true);
        return true;
    } else if (key == KEY_RIGHT) {
        m_pLists[m_curList]->setSelected(false);
        m_curList = (m_curList + 1 >= m_numCols) ? 0 : m_curList + 1;
        m_pLists[m_curList]->setSelected(true);        
        return true;
    } else {
        int numRows = m_pLists[m_curList]->getNumRows();
        bool b = m_pLists[m_curList]->handleKeyPress(key);
        if (numRows < m_pLists[m_curList]->getNumRows()) {
            for (int i = 0; i < m_numCols; i++) {
                if (i != m_curList) {
                    m_pLists[i]->addRow("");
                }
            }
        }
        return b;
    }
}

void Table::addRow(const std::string& str) {

}