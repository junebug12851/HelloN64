#include <nusys.h>
#include "stage_manager.h"
#include "stage_list.h"

// Contains all the stages in numerical order
struct StageInterface allStages[STAGE_COUNT];

// Points to the current stage
struct StageInterface curStage;

// Switches to a new stage
void stageSwitch(int ind)
{
	// Destruct old stage
	curStage.stageDestruct();

	// Change active stages
	curStage = allStages[ind];

	// Init new stage
	curStage.stageInit();
}

// Should be called once to init all the stages
void stageManagerInit()
{
	// Load stages into array
	allStages[STAGE_STARTUP] = stageStartup;

	// Start at startup stage
	curStage = allStages[STAGE_STARTUP];

	// Init it
	curStage.stageInit();
}
