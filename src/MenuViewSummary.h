//
//  MenuViewSummary.h
//  CuTAES
//
//  Created by Zack Dawson on 13-02-13.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#ifndef MENU_VIEW_SUMMARY_H
#define MENU_VIEW_SUMMARY_H

#include "Panel.h"

class MenuViewSummary : public Panel {
public:
    MenuViewSummary(const std::string& title, const std::string& course);
    virtual ~MenuViewSummary();
    
    virtual bool handleKeyPress(int key);
};

#endif //MENU_VIEW_SUMMARY_H
