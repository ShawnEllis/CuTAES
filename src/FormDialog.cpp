#include "FormDialog.h"

#include "CuTAES.h"

FormDialog::FormDialog(const std::string &title, int numFields) : Panel(title), m_numFields(numFields) {
    //Create field array, init to null
    m_pFields = new FIELD*[numFields];
    m_pLabels = new std::string[numFields];
    for (int i = 0; i < numFields; i++) {
        m_pFields[i] = 0;
        m_pLabels[i] = "";
    }
    //Create form & window
    m_pForm = new_form(&m_pFields[0]);
    scale_form(m_pForm, &m_rows, &m_cols);
    m_pWindow = newwin(m_rows + 5, m_cols + 4, 4, CuTAES::DEF_W / 2 - (m_cols + 4) / 2);

}

FormDialog::~FormDialog() {
    
}

void FormDialog::addField(const std::string &label, int rows, int cols, int x, int y, int type) {
    set_field_back(m_pFields[0], A_UNDERLINE);
    field_opts_off(m_pFields[0], O_AUTOSKIP);
}

void FormDialog::show() {
    set_form_win(m_pForm, m_pWindow);
    set_form_sub(m_pForm, derwin(m_pWindow, m_rows, m_cols, 2, 2));
    box(m_pWindow, 0, 0);
    
    post_form(m_pForm);
    
    form_driver(m_pForm, REQ_END_LINE);
    curs_set(1);
    
    wrefresh(m_pWindow);
    waitForInput();
}

void FormDialog::waitForInput() {
    int key;
    while ((key = wgetch(m_pWindow)) != 96) { //Control key cancels
        if (key == KEY_UP) {
            //Move up a field
            form_driver(m_pForm, REQ_PREV_FIELD);
            form_driver(m_pForm, REQ_END_LINE);
            wrefresh(m_pWindow);
        } else if (key == KEY_DOWN || key == 9) { //Key 9 is tab
            //Move down a field
            form_driver(m_pForm, REQ_NEXT_FIELD);
            form_driver(m_pForm, REQ_END_LINE);
            wrefresh(m_pWindow);
        } else if (key == KEY_LEFT) {
            //Move left in field
            form_driver(m_pForm, REQ_LEFT_CHAR);
            wrefresh(m_pWindow);
        } else if (key == KEY_RIGHT) {
            //TODO: Stop right arrow working as spacebar
            //Move right in field
            form_driver(m_pForm, REQ_RIGHT_CHAR);
            wrefresh(m_pWindow);
        } else if (key == KEY_BACKSPACE || key == 127) {
            //Delete prev. char
            form_driver(m_pForm, REQ_DEL_PREV);
            wrefresh(m_pWindow);
        } else if (key == CuTAES::KEY_ENT) {
            //TODO: Accept form, if data in fields are valid
        } else {
            //Send key to form driver
            form_driver(m_pForm, key);
            wrefresh(m_pWindow);
        }
    }
}