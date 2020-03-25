#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

#include <nusys.h>

extern NUContData controllerData[4];
extern u8 controllerPattern;

void controllerInit();
void controllerRefreshButtonState();
u8 controllerCheckButton(int btnState);

#endif
