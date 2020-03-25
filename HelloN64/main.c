/*========================================================
                Initialize the game
========================================================*/
/*=================================
            Libraries
=================================*/

#include <nusys.h>
#include "config.h"
#include "stages.h"

/*=================================
             Prototypes
=================================*/

static void vsyncCallback(int pendingTaskCount);
static void callback_prenmi();

/*=================================
         Global Variables
=================================*/

NUContData contdata[1];      // Player 1 Controller data
u8 contPattern;              // Controller Manager

/*=================================
              Main
=================================*/

void mainproc(void* dummy)
{
    // Start by selecting the proper television
    if (TV_TYPE == PAL)
    {
        // Set video mode to Full-Screen PAL
        // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
        osViSetMode(&osViModeTable[OS_VI_FPAL_LAN1]);

        // Squish image slightly
        // If Pre-NMI event occurs (Reset Button) this will crash if not fixed back correctly
        osViSetYScale(0.833); // Dirty but works

        // Register call-back function for Pre-NMI Event, we need to re-correct the above 
        // Y-Scale or the game will crash instead of resetting
        nuPreNMIFuncSet((NUScPreNMIFunc)callback_prenmi);
    }
    else if (TV_TYPE == MPAL)
        // Nothing fancy here, just set to brazilian format
        // (L)ow-Resolution, (A)nti-Aliasing, (N)on-Interlaced, (1)6-bit
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);

    // Initialize and activate the graphics thread and Graphics Task Manager.
    // Sort of completes booting of the NuSys N64 OS
    nuGfxInit();
    contPattern = nuContInit();

    // Tell NuSys to call this function on every vsync (I'm presuming "frame" or when the scanline
    // is in the vblank area on TV output before it moves back to the top)
    nuGfxFuncSet((NUGfxFunc)vsyncCallback);

    // Initialize stage 0
    stage00_init();

    // Turn on the screen
    nuGfxDisplayOn();

    // Loop forever to keep the idle thread busy
    while (1)
        ;
}

/*=================================
          vsyncCallback
This is a callback functions which
  execute instructions, and once
it has run out of instructions, it
  activates the display process.
=================================*/

// I'm assumign what the above means is that it prepares and begins transmitting the
// instructions to the RCP, when done transmitting and executing the instructions
// it starts transmission a new batch of instructions.

void vsyncCallback(int pendingTaskCount)
{
    // Update the stage throughout
    stage00_update();

    // If we've run out of tasks, draw the stage
    // I'm assuming this means transmit a new batch of instructions
    // because the previous batch has completed transmitting and executing.
    if (pendingTaskCount < 1)
        stage00_draw();
}

// To prevent crashing when the reset button is pressed
// This is only called when compiled in PAL Mode
void callback_prenmi()
{
    nuGfxDisplayOff();
    osViSetYScale(1);
}
