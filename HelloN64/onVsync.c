#include <nusys.h>
#include "stageManager.h"

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

void onVSync(int pendingTaskCount)
{
    // Update the stage throughout
    stage_update();

    // If we've run out of tasks, draw the stage
    // I'm assuming this means transmit a new batch of instructions
    // because the previous batch has completed transmitting and executing.
    if (pendingTaskCount < 1)
        stage_draw();
}
