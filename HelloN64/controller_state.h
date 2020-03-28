#ifndef CONTROLLER_STATE_H
#define CONTROLLER_STATE_H

#include <nusys.h>

#define CONTROLLER_1 (1 << 0)
#define CONTROLLER_2 (1 << 1)
#define CONTROLLER_3 (1 << 2)
#define CONTROLLER_4 (1 << 3)

// Max Joy Values
#define CONTROLLER_JOY_RNG_X 61
#define CONTROLLER_JOY_RNG_Y 63
#define CONTROLLER_JOY_RNG_ANG_X 45
#define CONTROLLER_JOY_RNG_ANG_Y 47

// Deadzone Percent
#define CONTROLLER_JOY_DEADZONE_PERCENT 0.05

#define CONTROLLER_JOY_DEADZONE_X (CONTROLLER_JOY_DEADZONE_PERCENT * CONTROLLER_JOY_RNG_X)
#define CONTROLLER_JOY_DEADZONE_Y (CONTROLLER_JOY_DEADZONE_PERCENT * CONTROLLER_JOY_RNG_Y)

struct MotionReturn
{
	// Motion percent 0.0 - 1.0
	float x;
	float y;
};

extern NUContData controllerData[4];
extern u16 controllerBtnDown[4];
extern u16 controllerBtnReleased[4];
extern u16 controllerBtnPressed[4];
extern u8 controllerConfig;

void controllerInit();
void controllerRefreshButtonState();

u8 controllerMatchConfig(int reqControllerConfig);
u8 controllerAtLeastConfig(int reqControllerConfig);
u8 controllerAtLeast1();
u8 controllerCount();

u8 controllerBtnDownMatchConfig(int controller, int reqControllerBtnConfig);
u8 controllerBtnDownAtLeastConfig(int controller, int reqControllerBtnConfig);
u8 controllerBtnDownAtLeast1(int controller);
u8 controllerBtnDownCount(int controller);

u8 controllerBtnPressedMatchConfig(int controller, int reqControllerBtnConfig);
u8 controllerBtnPressedAtLeastConfig(int controller, int reqControllerBtnConfig);
u8 controllerBtnPressedAtLeast1(int controller);
u8 controllerBtnPressedCount(int controller);

u8 controllerBtnReleasedMatchConfig(int controller, int reqControllerBtnConfig);
u8 controllerBtnReleasedAtLeastConfig(int controller, int reqControllerBtnConfig);
u8 controllerBtnReleasedAtLeast1(int controller);
u8 controllerBtnReleasedCount(int controller);

struct MotionReturn controllerJoyMotion(int controller);

#endif
