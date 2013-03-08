#include "DialogForm.h"
#include <sstream>

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

DialogForm::DialogForm(const std::string &title, int numFields) : Panel(title), m_numFields(numFields) {
    m_curField = 0;
    m_rows = 0;
    m_cols = 0;
    
    //Create field array, init to null
    m_pFields = new FIELD*[numFields + 1];
    for (int i = 0; i < numFields; i++) {
        m_pFields[i] = 0;
    }
    
    setReturnState(STATE_ERROR); //Default return is error
}

DialogForm::~DialogForm() {
    unpost_form(m_pForm);
//    free_form(m_pForm); TODO: Fix segfault
    for (int i = 0; i < m_numFields; i++) {
        if (m_pFields[i] != 0) {
            void *usrPtr = field_userptr(m_pFields[i]);
            if (usrPtr != 0) {
                delete (FieldType*)usrPtr; //TODO: Fix potentially unsafe conversion
            }
            free_field(m_pFields[i]);
        }
    }
    curs_set(0);
    refresh();
}

StateType DialogForm::show() {    
    if (!m_pFields[0]) {
        return STATE_ERROR;
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
    delwin(m_pWindow);
    m_pWindow = newwin(m_rows + 6, m_cols + 4, (getmaxy(stdscr) - m_rows) / 2 - 3, (getmaxx(stdscr) - m_cols) / 2 - 2);
    
    set_form_win(m_pForm, m_pWindow);
    set_form_sub(m_pForm, derwin(m_pWindow, m_rows, m_cols, 2, 2));
    
    post_form(m_pForm);
    
    curs_set(1);
    Panel::show();
    curs_set(0);
    return getReturnState();
}

void DialogForm::draw() {
    box(m_pWindow, 0, 0);
    
    //Draw title
    mvwprintw(m_pWindow, 1, 1, getTitle().data());
    WindowUtil::drawHLine(m_pWindow, 1, 2, m_cols + 1);
    
    drawComponents();
    
    //Draw cancel / ok commands
    
    mvwprintw(m_pWindow, m_rows + 4, 1, "Cancel: F3");
	mvwprintw(m_pWindow, m_rows + 4, m_cols - 9, "Continue: F2");
 
    form_driver(m_pForm, REQ_FIRST_FIELD);
    form_driver(m_pForm, REQ_END_LINE);
    
    wrefresh(m_pWindow);
}

/*
 * Overrides Panel's waitForInput().
 */
bool DialogForm::handleKeyPress(int key) {
    if (key == KEY_UP) {
        //Move up a field
        isFieldValid();
        form_driver(m_pForm, REQ_UP_FIELD);
        form_driver(m_pForm, REQ_END_LINE);
    }
    else if (key == KEY_DOWN || key == 9) {
        //Move down a field
        isFieldValid();
        form_driver(m_pForm, REQ_NEXT_FIELD);
        form_driver(m_pForm, REQ_END_LINE);    
    }
    else if (key == KEY_LEFT) {
        //Move left in field
        if (form_driver(m_pForm, REQ_LEFT_CHAR) != E_OK) {
            isFieldValid();
            form_driver(m_pForm, REQ_LEFT_FIELD);
            form_driver(m_pForm, REQ_END_LINE);
        }
    }
    else if (key == KEY_RIGHT) {
        //TODO: Stop right arrow working as spacebar
        //Move right in field
        if (form_driver(m_pForm, REQ_RIGHT_CHAR) != E_OK) {
            isFieldValid();
            form_driver(m_pForm, REQ_RIGHT_FIELD);
            form_driver(m_pForm, REQ_END_LINE);
        }  
    }
    else if (key == 9) { //Tab key
        //Move to next field
        isFieldValid();
        form_driver(m_pForm, REQ_NEXT_FIELD);
        form_driver(m_pForm, REQ_END_LINE);
    }
    else if (key == KEY_BACKSPACE || key == 127) {
        //Delete prev. char
        form_driver(m_pForm, REQ_DEL_PREV);
    }
    else if (key == KEY_F(2)) {
        if (isDataValid()) {
            setReturnState(STATE_SUCCESS);
            hide();
            return true;
        }
    }
    else if (key == KEY_F(3)) {
        setReturnState(STATE_CANCEL);
        hide();
    }
    else {
        //Send key to form driver
        form_driver(m_pForm, key);
    }
    
    return true; //Always consume the event
}

bool DialogForm::isFieldValid() {
    FIELD* pField = current_field(m_pForm);
    if (pField == 0) {
        return true;
    }
    //Get field type
    void* usrPtr = field_userptr(pField);
    FieldType type = usrPtr == 0 ? FIELDTYPE_NONE : *(FieldType*)usrPtr;
    //Get field text
    std::string fieldText = field_buffer(pField, 0);
    StringUtil::trimEnd(fieldText);
    
    //Check if field is valid TODO: Add own validation
    if (form_driver(m_pForm, REQ_VALIDATION) == E_INVALID_FIELD || (type == FIELDTYPE_EMAIL && !isValidEmail(fieldText)) || (type == FIELDTYPE_NAME && !isValidName(fieldText))) {
        //Field is invalid, highlight it
        set_field_back(current_field(m_pForm), A_STANDOUT);
        return false;
    }
    set_field_back(current_field(m_pForm), A_UNDERLINE);
    return true;
}

bool DialogForm::isDataValid() {
    for (int i = 0; i < m_numFields; i++) {
        std::string str = field_buffer(m_pFields[i], 0);
        StringUtil::trimEnd(str);
        if (!isFieldValid() || str.compare("") == 0) {
            return false;
        }
    }
    return true;
}

bool DialogForm::getFormData(std::string **pData) {
    if (getReturnState() != STATE_SUCCESS || pData == 0 || *pData != 0) {
        return false;
    }
#ifdef DEBUG
    dout << "Form " << getTitle() << " data: " << std::endl;
#endif //DEBUG
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
    return true;
}

void DialogForm::addField(const std::string &label, int rows, int cols, FieldType type, int *typeParams, int nParams) {
    addField(label, rows, cols, 0, m_curField * 2, type, typeParams, nParams);
}

void DialogForm::addField(const std::string &label, const std::string &txt, int rows, int cols, FieldType type, int *typeParams, int nParams) {
    std::stringstream ss;
    ss << label << "|" << txt;
    addField(ss.str(), rows, cols, 0, m_curField * 2, type, typeParams, nParams);
}

void DialogForm::addField(const std::string &label, const std::string &txt, int rows, int cols, int x, int y, FieldType type, int *typeParams, int nParams, FIELD** pField) {
    std::stringstream ss;
    ss << label << "|" << txt;
    addField(ss.str(), rows, cols, x, y, type, typeParams, nParams, pField);
}

/*
 * Add a field to the form.
 * Automatically creates a label component.
 * Include | in your label to designate a field's initial content.
 */
void DialogForm::addField(const std::string &lbl, int rows, int cols, int x, int y, FieldType type, int *typeParams, int nParams, FIELD** pField) {    
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
        if (typeParams == 0) {
            set_field_type(m_pFields[m_curField], TYPE_NUMERIC, 0);
        } else if (nParams == 1) {
            set_field_type(m_pFields[m_curField], TYPE_NUMERIC, typeParams[0]);
        } else if (nParams == 3) {
//            set_field_type(m_pFields[m_curField], TYPE_NUMERIC, typeParams[0], typeParams[1], typeParams[2]);
        }
    }
    set_field_userptr(m_pFields[m_curField], (void*)new FieldType(type));
    
    set_field_back(m_pFields[m_curField], A_UNDERLINE);
    if (rows == 1) {
        field_opts_off(m_pFields[m_curField], O_AUTOSKIP);
    }
    
    //Create the label
    Label* pLabel = new Label(this, label, x + 1, y + 3);
    add(pLabel);
    
    //Return pointer
    if (pField != 0) {
        *pField = m_pFields[m_curField];
    }
    
    m_curField++;
}

bool DialogForm::isValidName(const std::string text){
    std::string str = text;
    unsigned found =str.find_first_not_of("abcdefghijklmnopqrstuvwxyz'- ");
    if (found != std::string::npos && str.length() > 0)
        return false;
    return true;
}

bool DialogForm::isValidEmail(const std::string text){
    std::string str = text;
    unsigned found = str.find_first_not_of("abcdefghijklmnopqrstuvwxyz1234567890_-@.");
    if (found != std::string::npos)
        return false;
    found = str.find_first_of('@');
    if ((found == std::string::npos || str.find_first_of('@', found+1) != std::string::npos) && str.length() > 0)
	return false;
    return true;
}
