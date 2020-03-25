#include <nusys.h>
#include "stages.h"

// Calls the correct stage, defaults to stage #0
void (*stage_init)() = stage00_init;
void (*stage_update)() = stage00_update;
void (*stage_draw)() = stage00_draw;
