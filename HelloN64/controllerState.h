#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

#include <nusys.h>

extern NUContData contdata[4];
extern u8 contPattern;

void controller_init();
void controller_refreshButtonState();
u8 controller_checkButton(int btnState);

#endif
