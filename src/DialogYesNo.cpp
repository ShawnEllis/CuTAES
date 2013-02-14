#include "DialogYesNo.h"
#include <stdlib.h>
#include <string>
#include "Button.h"

DialogYesNo::DialogYesNo(const std::string& msg) : Panel(msg, std::max(20, (int)msg.size() + 2), 7) {
    setReturnState(STATE_ERROR); //Default return is error
    
    Button *pButton = new Button(this, "Yes", getWidth() / 2 + 6, getHeight() - 3, 0, 0);
    pButton->setEventHandler(handleYesPressed);
    pButton->setUsrPtr(this);
    add(pButton);
    
    pButton = new Button(this, "No", getWidth() / 2 - 6, getHeight() - 3, 0, 0);
    pButton->setEventHandler(handleNoPressed);
    pButton->setUsrPtr(this);
    add(pButton);
}

DialogYesNo::~DialogYesNo() {
}

void DialogYesNo::handleNoPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        DialogYesNo *pDia = ((DialogYesNo*)usrPtr);
        pDia->setReturnState(STATE_CANCEL);
        pDia->hide();
    }
}

void DialogYesNo::handleYesPressed(Button *pButton) {
    if (pButton == 0) {
        return;
    }
    void* usrPtr = pButton->getUsrPtr();
    if (usrPtr != 0) {
        DialogYesNo *pDia = ((DialogYesNo*)usrPtr);
        pDia->setReturnState(STATE_SUCCESS);
        pDia->hide();
    }
}