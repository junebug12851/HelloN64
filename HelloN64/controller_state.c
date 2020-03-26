#include <nusys.h>
#include "controller_state.h"

NUContData controllerData[4];      // Player 1 Controller data
u8 controllerConfig;              // Controller Manager

void controllerInit()
{
	// Initialize the controllers
	controllerConfig = nuContInit();
}

void controllerRefreshButtonState()
{
	// Get controller input
	nuContDataGetExAll(controllerData);
}

// Given a requested config, returns if the config matches the requested config exactly
u8 controllerMatchConfig(int reqControllerConfig)
{
	return (controllerConfig & reqControllerConfig) == reqControllerConfig;
}

// Given a requested config, returns if at least one controller is plugged into
// requested config
u8 controllerAtLeastConfig(int reqControllerConfig)
{
	return (controllerConfig & reqControllerConfig) > 0;
}

// Returns if at least 1 controller is plugged in
u8 controllerAtLeast1()
{
	return controllerConfig > 0;
}

u8 controllerCount()
{
	int ret = 0;

	if (controllerConfig & CONTROLLER_1)
		ret++;
	else if (controllerConfig & CONTROLLER_2)
		ret++;
	else if (controllerConfig & CONTROLLER_3)
		ret++;
	else if (controllerConfig & CONTROLLER_4)
		ret++;

	return ret;
}

// Given a requested config, returns if the config matches the requested config exactly
u8 controllerBtnMatchConfig(int controller, int reqControllerBtnConfig)
{
	NUContData* data = &controllerData[controller];
	return (data->button & reqControllerBtnConfig) == reqControllerBtnConfig;
}

// Given a requested config, returns if at least one controller is plugged into
// requested config
u8 controllerBtnAtLeastConfig(int controller, int reqControllerBtnConfig)
{
	NUContData* data = &controllerData[controller];
	return (data->button & reqControllerBtnConfig) > 0;
}

// Returns if at least 1 controller is plugged in
u8 controllerBtnAtLeast1(int controller)
{
	NUContData* data = &controllerData[controller];
	return data->button > 0;
}

u8 controllerBtnCount(int controller)
{
	int ret = 0;
	NUContData* data = &controllerData[controller];

	u8 i = 0;
	for(i = 0; i < 16; i++)
	{
		u16 bit = 1 << i;
		if (bit & data->button)
			ret++;
	}

	return ret;
}

// Given a requested config, returns if the config matches the requested config exactly
u8 controllerTrigMatchConfig(int controller, int reqControllerBtnConfig)
{
	NUContData* data = &controllerData[controller];
	return (data->trigger & reqControllerBtnConfig) == reqControllerBtnConfig;
}

// Given a requested config, returns if at least one controller is plugged into
// requested config
u8 controllerTrigAtLeastConfig(int controller, int reqControllerBtnConfig)
{
	NUContData* data = &controllerData[controller];
	return (data->trigger & reqControllerBtnConfig) > 0;
}

// Returns if at least 1 controller is plugged in
u8 controllerTrigAtLeast1(int controller)
{
	NUContData* data = &controllerData[controller];
	return data->trigger > 0;
}

u8 controllerTrigCount(int controller)
{
	int ret = 0;
	NUContData* data = &controllerData[controller];

	u8 i = 0;
	for (i = 0; i < 16; i++)
	{
		u16 bit = 1 << i;
		if (bit & data->trigger)
			ret++;
	}

	return ret;
}

//struct MotionReturn controllerJoyMotion(int controller)
//{
//	// Grab Controller Data
//	NUContData* data = &controllerData[controller];
//
//	// Grab Raw Joy Data
//	s8 x = data->stick_x;
//	s8 y = -data->stick_y; // Y is actually inverted, we need to correct it
//	u8 angled = 0;
//
//	// Prepare return ranges between 0.0 and 1.0
//	float retX = 0.0f;
//	float retY = 0.0f;
//
//	// Load into a return value
//	struct MotionReturn ret;
//
//	//	// Clamp X & Y to both a deadzone and max ranges
//	if (x <= CONTROLLER_JOY_DEADZONE_X && x >= -CONTROLLER_JOY_DEADZONE_X)
//		x = 0;
//	else if (x >= CONTROLLER_JOY_RNG_X)
//		x = CONTROLLER_JOY_RNG_X;
//	else if (x <= -CONTROLLER_JOY_RNG_X)
//		x = -CONTROLLER_JOY_RNG_X;
//
//	if (y <= CONTROLLER_JOY_DEADZONE_Y && y >= -CONTROLLER_JOY_DEADZONE_Y)
//		y = 0;
//	else if (y >= CONTROLLER_JOY_RNG_Y)
//		y = CONTROLLER_JOY_RNG_Y;
//	else if (y <= -CONTROLLER_JOY_RNG_Y)
//		y = -CONTROLLER_JOY_RNG_Y;
//
//	retX = x / CONTROLLER_JOY_RNG_X;
//	retY = y / CONTROLLER_JOY_RNG_Y;
//
//	ret.x = retX;
//	ret.y = retY;
//
//	// Return motion
//	return ret;
//}

// Gets data from the joystick as floats in the range of 0.00 - 1.00
// Takes into account deadzone and proper max ranges including angled ranges.
// Data is clamped to 1.00
struct MotionReturn controllerJoyMotion(int controller)
{
	// Grab Controller Data
	NUContData* data = &controllerData[controller];

	// Grab Raw Joy Data
	s8 x = data->stick_x;
	s8 y = -data->stick_y; // Y is actually inverted, we need to correct it
	u8 angled = 0;

	// Prepare return ranges between 0.0 and 1.0
	float retX = 0.0f;
	float retY = 0.0f;

	// Load into a return value
	struct MotionReturn ret;

	// Clamp X & Y to both a deadzone and max ranges
	if (x <= CONTROLLER_JOY_DEADZONE_X && x >= -CONTROLLER_JOY_DEADZONE_X)
		x = 0;
	else if (x >= CONTROLLER_JOY_RNG_X)
		x = CONTROLLER_JOY_RNG_X;
	else if (x <= -CONTROLLER_JOY_RNG_X)
		x = -CONTROLLER_JOY_RNG_X;

	if (y <= CONTROLLER_JOY_DEADZONE_Y && y >= -CONTROLLER_JOY_DEADZONE_Y)
		y = 0;
	else if (y >= CONTROLLER_JOY_RNG_Y)
		y = CONTROLLER_JOY_RNG_Y;
	else if (y <= -CONTROLLER_JOY_RNG_Y)
		y = -CONTROLLER_JOY_RNG_Y;

	// Mark if it's angled, angles have a narrower range
	if ((x > 0 || x < 0) && 
		(y > 0 || y < 0))
		angled = 1;

	// Convert the number to a range depending on angle
	if (angled == 0) {
		retX = x / CONTROLLER_JOY_RNG_X;
		retY = y / CONTROLLER_JOY_RNG_Y;
	}
	else {
		retX = x / CONTROLLER_JOY_RNG_ANG_X;
		retY = y / CONTROLLER_JOY_RNG_ANG_Y;
	}

	// Cap it at 1.00 if it's past 1.00
	if (retX > 1.00f)
		retX = 1.00f;
	else if (retX < -1.00f)
		retX = -1.00f;

	if (retY > 1.00f)
		retY = 1.00f;
	else if (retY < -1.00f)
		retY = -1.00f;

	ret.x = retX;
	ret.y = retY;

	// Return motion
	return ret;
}
