/*
 *  DialogForm.h
 *
 *  A highly configurable dialog which prompts the user for various data.
 * 
 *  Usage:
 *  Create a new instance with the number of fields you will add.
 *  Then, individually add each field with addField().
 *  Once the form is set up, call show() to get an array of data
 *  corresponding to the fields you added, in order of addition.
 *
 */

#ifndef DialogForm_H
#define DialogForm_H

#include "Panel.h"
#include <string>
#include <curses.h>
#include <form.h>
#include "StringUtil.h"

class DialogForm : public Panel {
public:
    DialogForm(const std::string &title, int numFields);
    virtual ~DialogForm();
    
    virtual StateType show();
    virtual void draw();
    
    virtual bool handleKeyPress(int key);
    
    bool getFormData(std::string **pData);
    
    void addField(const std::string &label, int rows, int cols, int type=0, int *typeParams=0, int nParams=0);
    void addField(const std::string &label, int rows, int cols, int x, int y, int type=0, int *typeParams=0, int nParams=0, FIELD** pField=0);
    
    void addList(const std::string& label, int width, int height, int x, int y);
    
private:
    int m_numFields, m_curField; //Total and current number of fields
    int m_rows, m_cols;
    
    FIELD **m_pFields;
    FORM *m_pForm;
    
    bool isDataValid();
    bool isFieldValid();
};

enum {
    FIELDTYPE_NONE = 0,
    FIELDTYPE_ALPHA,
    FIELDTYPE_INT,
    FIELDTYPE_FLOAT
};


#endif //DialogForm_H
