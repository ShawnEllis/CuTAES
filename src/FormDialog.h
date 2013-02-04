#ifndef FORMDIALOG_H
#define FORMDIALOG_H

#include "Panel.h"
#include <string>
#include <curses.h>
#include <form.h>

class FormDialog : Panel {
public:
    FormDialog(const std::string &title, int numFields);
    virtual ~FormDialog();
    
    void addField(const std::string &label, int rows, int cols, int x, int y, int type);
    
    void show();
    void waitForInput();
    
private:
    int m_numFields;
    int m_rows, m_cols;
    
    std::string *m_pLabels;
    FIELD **m_pFields;
    FORM *m_pForm;
    WINDOW *m_pWindow;
};


#endif //FORMDIALOG_H
