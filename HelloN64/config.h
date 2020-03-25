#ifndef N64_CONFIG_H
#define N64_CONFIG_H

#include <nusys.h>

#define NTSC    0
#define PAL     1
#define MPAL    2

#define TV_TYPE NTSC

#define SCREEN_WD   320
#define SCREEN_HT   240

#define GLIST_LENGTH    (2048)

#define CheckController(cont) (contPattern & (1<<(cont)))

// Holds list of instructions to transmit to the RCP
// and a pointer of position
Gfx glist[GLIST_LENGTH];
Gfx* glistp;

// Prepares RCP for incomming work
void RCPInit(Gfx* glistp);

// Controller Data
extern NUContData contdata[1];
extern u8 contPattern;

#define checkController(cont) (contPattern & (1<<(cont)))

#endif
