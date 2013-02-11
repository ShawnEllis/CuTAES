#ifndef TABLE_H
#define TABLE_H

#include "Component.h"
#include <string>

class ListBox;

class Table : public Component {
public:
    Table(Panel *pPanel, int x, int y, int colH, int numCols, int *colWidths, std::string* labels);
    virtual ~Table();
    
    virtual void draw();
    
    virtual bool handleKeyPress(int key);
    
private:
    void addRow(const std::string &label="New Row");

    int m_numCols;
    int *m_colWidths;
    
    int m_curList;
    ListBox **m_pLists;
};

#endif //TABLE_H