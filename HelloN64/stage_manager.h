#ifndef STAGE_MANAGER_H
#define STAGE_MANAGER_H

typedef void (*StageInitFunc)();
typedef void (*StageUpdateFunc)();
typedef void (*StageDrawFunc)();
typedef void (*StageDestructFunc)();

struct StageInterface
{
	StageInitFunc stageInit;
	StageUpdateFunc stageUpdate;
	StageDrawFunc stageDraw;
	StageDestructFunc stageDestruct;
};

extern struct StageInterface curStage;

void stageSwitch(int ind);
void stageManagerInit();

#endif // !STAGE_MANAGER_H
