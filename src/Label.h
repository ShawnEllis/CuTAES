/*
 * A simple text-label component.
 */

#ifndef LABEL_H
#define LABEL_H

#include "Component.h"
#include <string>

class Label: public Component {
    
public:
    Label(Panel *pPanel, const std::string& text, int x=0, int y=0);
    
    virtual ~Label();
    
    virtual void draw();
    
private:
    std::string m_text;
};


#endif
