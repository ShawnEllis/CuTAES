#include "FormDialog.h"

#include "Component.h"
#include "ListNode.h"
#include "Label.h"
#include "ListBox.h"
#include "WindowUtil.h"

#ifdef DEBUG
#include <iostream>
#include <fstream>
extern std::ofstream dout;
#endif //DEBUG

FormDialog::FormDialog(const std::string &title, int numFields) : Panel(title), m_numFields(numFields) {
    m_curField = 0;
    m_formAccepted = false;
    m_rows = 0;
    m_cols = 0;
    //Create field array, init to null
    m_pFields = new FIELD*[numFields + 1];
    for (int i = 0; i < numFields; i++) {
        m_pFields[i] = 0;
    }
}

FormDialog::~FormDialog() {
    unpost_form(m_pForm);
    free_form(m_pForm);
    for (int i = 0; i < m_numFields; i++) {
        if (m_pFields[i] != 0) {
            free_field(m_pFields[i]);
        }
    }
    refresh();
}

void FormDialog::show() {
    if (!m_pFields[0]) {
        return;
    }
    //Create form & window
    FIELD *pFields[m_curField];
    for (int i = 0; i < m_numFields + 1; i++) {
        if (i < m_curField) {
            pFields[i] = m_pFields[i];
        } else {
            pFields[i] = 0;
        }
    }
    
    m_pForm = new_form(pFields);
    int rows, cols;
    scale_form(m_pForm, &rows, &cols);
    m_rows = std::max(m_rows, rows);
    m_cols = std::max(m_cols, cols);
    m_pWindow = newwin(m_rows + 6, m_cols + 4, 4, CuTAES::DEF_W / 2 - (m_cols + 4) / 2);
    
    set_form_win(m_pForm, m_pWindow);
    set_form_sub(m_pForm, derwin(m_pWindow, m_rows, m_cols, 2, 2));
    
    box(m_pWindow, 0, 0);
    post_form(m_pForm);
    
    //Draw title
    mvwprintw(m_pWindow, 1, 1, m_title.data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, m_cols + 1);
    
    drawComponents();
    
    //Draw cancel / ok commands
    
    mvwprintw(m_pWindow, m_rows + 4, 1, "`/~: Cancel");
	mvwprintw(m_pWindow, m_rows + 4, m_cols - 10, "ENTER: Accept");
    
    form_driver(m_pForm, REQ_END_LINE);
    
    curs_set(1);
    wrefresh(m_pWindow);
    waitForInput();
    curs_set(0);
}

/*
 * Overrides Panel's waitForInput().
 */
void FormDialog::waitForInput() {
    int key;
    while ((key = getch()) != 96) { //Control key cancels
        if (key == KEY_UP) {
            //Move up a field
            form_driver(m_pForm, REQ_PREV_FIELD);
            form_driver(m_pForm, REQ_END_LINE);
            wrefresh(m_pWindow);
        } else if (key == KEY_DOWN || key == 9) { //Tab key
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
        } else if (key == 330) { //Delete key TODO: Fix this
            //Delete next char
            form_driver(m_pForm, REQ_DEL_PREV);
            wrefresh(m_pWindow);
        } else if (key == CuTAES::KEY_ENT) {
            //TODO: Accept form, if data in fields are valid
            form_driver(m_pForm, REQ_NEXT_FIELD); //TODO: Find better way to validate cur field
            if (isDataValid()) {
                m_formAccepted = true;
                return;
            }
            form_driver(m_pForm, REQ_PREV_FIELD); //Stay on current line
        } else {
            //Send key to form driver
            form_driver(m_pForm, key);
            wrefresh(m_pWindow);
        }
    }
}

bool FormDialog::isDataValid() {
    for (int i = 0; i < m_numFields; i++) {
        std::string str = field_buffer(m_pFields[i], 0);
        StringUtil::trimEnd(str);
        if (str.compare("") != 0) {
            return true;
        }
    }
    return false;
}

void FormDialog::getFormData(bool *pAccepted, std::string **pData) {
#ifdef DEBUG
    dout << "Form " << m_title << " data: " << std::endl;
#endif //DEBUG
    *pAccepted = m_formAccepted;
    std::string* data = new std::string[m_numFields];
    for (int i = 0; i < m_numFields; i++) {
        std::string str = field_buffer(m_pFields[i], 0);
        StringUtil::trimEnd(str);
        data[i] = str;
#ifdef DEBUG
        dout << "   " << i << str << std::endl;
#endif //DEBUG
    }
    *pData = data;
}

void FormDialog::addField(const std::string &label, int rows, int cols, int type, int *typeParams, int nParams) {
    addField(label, rows, cols, 0, m_curField * 2, type, typeParams, nParams);
}

/*
 * Add a field to the form.
 * Automatically creates a label component.
 * Include | in your label to designate a field's initial content.
 */
void FormDialog::addField(const std::string &lbl, int rows, int cols, int x, int y, int type, int *typeParams, int nParams) {    
    //Split the label and field contents.
    int i;
    std::string contents, label;
    
    i = lbl.find_first_of('|');
    
    if (i != std::string::npos) {
        //Put content after | in field
        contents = lbl.substr(i + 1);
        label = lbl.substr(0, i);
    } else {
        contents = "";
        label = lbl;
    }
    
    //Create the field.
    if (rows == 1) {
        //Offset x by label strLen
        m_pFields[m_curField] = new_field(rows, cols, y + 1, label.size() + x, 0, 0);
    } else {
        //Position field under label
        m_pFields[m_curField] = new_field(rows, cols, y + 2, x, 0, 0);
    }
    set_field_buffer(m_pFields[m_curField], 0, contents.data());
    
    //Set up field validation.
    if (type == FIELDTYPE_ALPHA) {
        if (typeParams == 0) {
            set_field_type(m_pFields[m_curField], TYPE_ALPHA, 1);
        } else if (nParams == 1) {
            set_field_type(m_pFields[m_curField], TYPE_ALPHA, typeParams[0]);
        }
    } else if (type == FIELDTYPE_INT) {
        if (typeParams == 0) {
            set_field_type(m_pFields[m_curField], TYPE_INTEGER, 0);
        } else if (nParams == 1) {
            set_field_type(m_pFields[m_curField], TYPE_INTEGER, typeParams[0]);
        } else if (nParams == 3) {
            set_field_type(m_pFields[m_curField], TYPE_INTEGER, typeParams[0], typeParams[1], typeParams[2]);
        }
    } else if (type == FIELDTYPE_FLOAT) {
//        if (typeParams == 0) {
//            set_field_type(m_pFields[m_curField], TYPE_NUMERIC, 1);
//        } else if (nParams == 1) {
//            set_field_type(m_pFields[m_curField], TYPE_NUMERIC, typeParams[0]);
//        } else if (nParams == 3) {
//            set_field_type(m_pFields[m_curField], TYPE_NUMERIC, typeParams[0], typeParams[1], typeParams[2]);
//        }
    }
    
    set_field_back(m_pFields[m_curField], A_UNDERLINE);
    if (rows == 1) {
        field_opts_off(m_pFields[m_curField], O_AUTOSKIP);
    }
    
    //Create the label
    Label* pLabel = new Label(this, label, x + 1, y + 3);
    add(pLabel);
    
    m_curField++;
}

void FormDialog::addList(const std::string &label, int width, int height, int x, int y) {
    //Create the list
    ListBox *pList = new ListBox(this, x + 1, y + 4, width, height);
    add(pList);
    m_rows = std::max(m_rows, height + y);
    m_cols = std::max(m_cols, width + x);
    
    //Create the label
    Label* pLabel = new Label(this, label, x + 1, y + 3);
    add(pLabel);
}