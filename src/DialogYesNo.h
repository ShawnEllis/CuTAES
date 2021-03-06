/*
 *  DialogYesNo.h
 *
 *  A yes or no dialog with a customisable message.
 *
 */

#ifndef DIALOG_YESNO_H
#define DIALOG_YESNO_H

#include "Panel.h"

class Button;

enum DialogType {
    DIALOG_YESNO,
    DIALOG_MESSAGE
};

class DialogYesNo : public Panel {
public:
    DialogYesNo(const std::string& m_msg, DialogType type=DIALOG_YESNO);
    virtual ~DialogYesNo();
    
    static void handleNoPressed(Button * pButton);
    static void handleYesPressed(Button * pButton);
};

#endif //DIALOG_YESNO_H
