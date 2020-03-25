#include <nusys.h>
#include "gfx_glist.h"

// Holds list of instructions to transmit to the RCP
// and a pointer of position
Gfx glist[GLIST_LENGTH];
Gfx* glistp = glist;
