#include "ListBox.h"
#include <curses.h>

#include "Panel.h"
#include "WindowUtil.h"
#include "StringUtil.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

ListBox::ListBox(Panel *pPanel, int x, int y, int w, int h, bool editable) : Component(pPanel, x, y, w, h) {
    setSelectable(true);
    m_editable = true;//editable; TODO: revisit
    m_numRows = m_editable ? 1 : 0;
    m_pForm = 0;
    m_curRow = 0;
    
    //Create field array, init to null
    if (m_editable) {
        m_pFields = new FIELD*[2];
        createField(&m_pFields[0], 0, "...");
        set_field_userptr(m_pFields[0], (void*)true);
        m_pFields[1] = 0;
    } else {
        m_pFields = new FIELD*[1];
        m_pFields[0] = 0;
    }
    
    createForm();
}

ListBox::~ListBox() {
    unpost_form(m_pForm);
    free_form(m_pForm);
    for (int i = 0; i < m_numRows; i++) {
        if (m_pFields[i] != 0) {
            free_field(m_pFields[i]);
        }
    }
}

void ListBox::draw() {
    WindowUtil::drawRect(m_pPanel->getWindow(), getX(), getY(), getWidth(), getHeight());
}

bool ListBox::handleKeyPress(int key) {
    if (key == KEY_UP) {
        //Move up a field
        selectField(REQ_PREV_FIELD);
        m_curRow = (m_curRow == 0) ? m_numRows - 1 : m_curRow - 1;
        return true;
    } else if (key == KEY_DOWN || key == 9) { //Tab key
        //Move down a field
        selectField(REQ_NEXT_FIELD);
        m_curRow = (m_curRow == m_numRows - 1) ? 0 : m_curRow + 1;
        return true;
    } else if (key == KEY_BACKSPACE || key == 127) {
        //Delete prev. char
//        form_driver(m_pForm, REQ_DEL_PREV);
        return true;
    } else if (key == 330) { //Delete key TODO: Fix this
        //Delete next char
//        form_driver(m_pForm, REQ_DEL_PREV);
        return true;
    } else {
        FIELD *pField = current_field(m_pForm);
        if (field_userptr(pField) != 0) {
#ifdef DEBUG
            dout << "Create new row" << std::endl;
#endif
            addRow("");
            if (key == CuTAES::KEY_ENT) {
                //Clear field
                set_field_buffer(pField, 0, "");
                return true;
            }
        }
        //Send key to form driver
        form_driver(m_pForm, key);
    }
    wrefresh(m_pPanel->getWindow());
    return false;
}

std::string ListBox::getDataAt(int r) {
    std::string data;
    data = field_buffer(m_pFields[r], 0);
    StringUtil::trimEnd(data);
    return data;
}

/*
 *  Adds a row to the list, keeping the null row and '...' row in the correct positions.
 *  Must copy data to new array due to limitations in the forms library.
 */
void ListBox::addRow(const std::string& str) {
    FIELD **pNewFields = new FIELD*[m_numRows + (m_editable ? 2 : 1)];
    
    //Copy existing fields, up to the second last ('...') row
    for (int i = 0; i < m_numRows - 1; i++) {
        pNewFields[i] = m_pFields[i];
    }
    
    //Create '...' row
    if (m_editable) {
        //Create new field at third-to-last index (before '...' row)
        createField(&pNewFields[m_numRows - 1], m_numRows - 1, str);
        
        createField(&pNewFields[m_numRows], m_numRows, "...");
        set_field_userptr(pNewFields[m_numRows], (void*)true);
    } else {
        createField(&pNewFields[m_numRows], m_numRows, str);
    }
    
    //Create null field
    pNewFields[m_editable ? m_numRows + 1 : m_numRows] = 0;
    
    for (int i = 0; i < m_numRows; i++) {
        if (m_pFields[i] != 0) {
            free_field(m_pFields[i]);
        }
    }
    
    m_numRows++;
    
    m_pFields = pNewFields;
    
    createForm();
}

void ListBox::setCurRow(int r) {
    set_field_back(current_field(m_pForm), A_NORMAL);
    form_driver(m_pForm, REQ_FIRST_FIELD);
    for (int i = 0; i < r; i++) {
        form_driver(m_pForm, REQ_NEXT_FIELD);
    }
    form_driver(m_pForm, REQ_BEG_LINE);
    set_field_back(current_field(m_pForm), A_STANDOUT);
    m_curRow = r;
}

void ListBox::setSelected(bool sel) {
    Component::setSelected(sel);
    if (sel) {
        selectField(REQ_FIRST_FIELD);
    } else {
        set_field_back(current_field(m_pForm), A_NORMAL);
        form_driver(m_pForm, REQ_BEG_LINE);
    }
    m_curRow = 0;
    wrefresh(m_pPanel->getWindow());
}

/*
 *  Move using the given form driver request.
 *  De-highlights the old selection and highlights the new one.
 */
void ListBox::selectField(int formDriverReq) {
    //Unhighlight cur field
    set_field_back(current_field(m_pForm), A_NORMAL);
    form_driver(m_pForm, formDriverReq);
    form_driver(m_pForm, REQ_BEG_LINE);
    set_field_back(current_field(m_pForm), A_STANDOUT);
}

/*
 *  Creates a new field at y, and sets the field's text to str.
 *  Sets the value of pField to point to the new field.
 */
void ListBox::createField(FIELD **pField, int y, std::string str) {
    *pField = new_field(1, getWidth() - 2, y, 0, 0, 0);
    set_field_buffer(*pField, 0, str.data());
    set_field_back(*pField, A_NORMAL);
    field_opts_off(*pField, O_AUTOSKIP);
    field_opts_on(*pField, O_BLANK);
}

void ListBox::createForm() {
    if (m_pForm != 0) {
        unpost_form(m_pForm);
        free_form(m_pForm);
    }
    m_pForm = new_form(m_pFields);
    int rows, cols;
    scale_form(m_pForm, &rows, &cols);
    set_form_win(m_pForm, m_pPanel->getWindow());
    set_form_sub(m_pForm, derwin(m_pPanel->getWindow(), getHeight() - 2, getWidth() - 2, getY() + 1, getX() + 1));

    post_form(m_pForm);
    
    form_driver(m_pForm, REQ_LAST_FIELD);
    if (isSelected()) {
        selectField(REQ_PREV_FIELD);
    }
}

