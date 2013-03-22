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
//TODO import c isAlpha stuff
#include <form.h>
#include "StringUtil.h"

enum FieldType {
    FIELDTYPE_NONE,
    FIELDTYPE_ALPHA,
    FIELDTYPE_INT,
    FIELDTYPE_FLOAT,
    FIELDTYPE_EMAIL,
    FIELDTYPE_NAME
};

class DialogForm : public Panel {
public:
    DialogForm(const std::string &title, int numFields);
    virtual ~DialogForm();
    
    virtual StateType show();
    virtual void draw();
    
    virtual bool handleKeyPress(int key);
    
    bool getFormData(std::string **pData);
    
    void addField(const std::string &label, int rows, int cols, const FieldType type=FIELDTYPE_NONE, int *typeParams=0, int nParams=0);
    
    void addField(const std::string &label, const std::string &txt, int rows, int cols, const FieldType type=FIELDTYPE_NONE, int *typeParams=0, int nParams=0);
    
    void addField(const std::string &label, int rows, int cols, int x, int y, const FieldType type=FIELDTYPE_NONE, int *typeParams=0, int nParams=0, FIELD** pField=0);
    
    void addField(const std::string &label, const std::string& txt, int rows, int cols, int x, int y, const FieldType type=FIELDTYPE_NONE, int *typeParams=0, int nParams=0, FIELD** pField=0);
    
private:
    int m_numFields, m_curField; //Total and current number of fields
    int m_rows, m_cols;
    
    FIELD **m_pFields;
    FORM *m_pForm;
    
    bool isDataValid();
    bool isFieldValid();

    bool isValidName(const std::string& text);
    bool isValidEmail(const std::string& text);

};


#endif //DialogForm_H
