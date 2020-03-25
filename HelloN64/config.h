#ifndef N64_CONFIG_H
#define N64_CONFIG_H

#include <nusys.h>

#define SCREEN_WD   320
#define SCREEN_HT   240

#define GLIST_LENGTH    (2048)

// Holds list of instructions to transmit to the RCP
// and a pointer of position
Gfx glist[GLIST_LENGTH];
Gfx* glistp;

// Prepares RCP for incomming work
void RCPInit(Gfx* glistp);

#endif
