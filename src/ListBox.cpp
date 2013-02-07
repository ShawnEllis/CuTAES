#include "ListBox.h"
#include <curses.h>

#include "Panel.h"
#include "WindowUtil.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

ListBox::ListBox(Panel *pPanel, int x, int y, int w, int h) : Component(pPanel, x, y, w, h) {
    setSelectable(true);
    m_numRows = 1;
    
    //Create field array, init to null
    m_pFields = new FIELD*[2];
    createField(&m_pFields[0], 0, "...");
    m_pFields[1] = 0;
    
    m_pForm = new_form(m_pFields);
}

ListBox::~ListBox() {
    unpost_form(m_pForm);
    free_form(m_pForm);
    for (int i = 0; i < m_numRows; i++) {
        if (m_pFields[i] != 0) {
            free_field(m_pFields[i]);
        }
    }
    refresh();
}

void ListBox::draw() {
#ifdef DEBUG
    dout << "ListBox:draw()" << std::endl;
#endif //DEBUG
    if (isSelected()) {
        WindowUtil::fillRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight(), ' ');
    }
    WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight());
    int rows, cols;
    scale_form(m_pForm, &rows, &cols);
    set_form_win(m_pForm, m_pPanel->getWindow());
    set_form_sub(m_pForm, derwin(m_pPanel->getWindow(), getHeight() - 2, getWidth() - 2, getY() + 1, getX() + 1));
    post_form(m_pForm);
    wrefresh(m_pPanel->getWindow());
}

void ListBox::registerActionTriggers() {
    m_pPanel->registerAction(CuTAES::KEY_ENT, this, &Component::doAction);
}

void ListBox::doAction() {

}

/*
    Adds a row to the list, keeping the null row and '...' row in the correct positions.
    Must copy data to new array due to limitations in the forms library.
 */
void ListBox::addRow(const std::string& str) {
    FIELD **pNewFields = new FIELD*[m_numRows + 2]; //3
    
    //Copy existing fields, up to the second last ('...') row
    for (int i = 0; i < m_numRows - 1; i++) {
        pNewFields[i] = m_pFields[i];
    }
    
    //Create new field at third-to-last index (before '...' row)
    createField(&pNewFields[m_numRows - 1], m_numRows - 1, str);
    
    //Create '...' row
    createField(&pNewFields[m_numRows], m_numRows, "...");
    
    //Create null field
    pNewFields[m_numRows + 1] = 0;
    m_numRows++;
    
    m_pFields = pNewFields;
}

/*
 *  Creates a new field at y, and sets the field's text to str.
 *  Sets the value of pField to point to the new field.
 */
void ListBox::createField(FIELD **pField, int y, std::string str) {
    *pField = new_field(1, getWidth() - 2, y, 0, 0, 0);
    set_field_buffer(*pField, 0, str.data());
    set_field_back(*pField, A_UNDERLINE);
    field_opts_off(*pField, O_AUTOSKIP);
}