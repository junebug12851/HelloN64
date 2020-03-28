#include <nusys.h>
#include "controller_state.h"

// Normal controller data returned by NuSys
NUContData controllerData[4];			// Controller data

// Button state for buttons being held down starting the current frame
u16 controllerBtnDown[4];				// Data on Pressed Buttons

// Button state for buttons being pressed only in that frame
u16 controllerBtnPressed[4];			// Data on released Buttons

// Button state for buttons released only in that frame
u16 controllerBtnReleased[4];			// Data on released Buttons

// Controller State
u8 controllerConfig;					// Controller Manager

void controllerInit()
{
	u8 i;

	// Initialize the controllers
	controllerConfig = nuContInit();

	// Format release data
	for (i = 0; i < 4; i++)
	{
		controllerBtnDown[i] = 0;
		controllerBtnPressed[i] = 0;
		controllerBtnReleased[i] = 0;
	}
}

void controllerRefreshButtonState()
{
	// Check buttons
	u8 i;
	u8 j;

	// Get controller input
	nuContDataGetExAll(controllerData);

	for (i = 0; i < 4; i++) {
		for (j = 0; j < 16; j++)
		{
			// Grab the current bitmask
			u16 bitMask = 1 << j;

			// Check if a button was pressed that's not in this frame
			u8 current = (controllerData[i].trigger & bitMask) > 0; // Grab trigger data
			u8 previous = (controllerBtnDown[i] & bitMask) > 0;

			// current is not reliable unless you also factor in button data
			current |= (controllerData[i].button & bitMask) > 0;

			// If a button is newly held down, mark it as now being pressed
			if (current && !previous) {
				controllerBtnDown[i] |= bitMask;
				controllerBtnPressed[i] |= bitMask;
			}
			// If a button was being held down, but is now released, mark it as released
			// only for this frame. Also unmark it as being pressed.
			else if (!current && previous) {
				controllerBtnReleased[i] |= bitMask;
				controllerBtnDown[i] &= ~bitMask;
			}

			// Otherwise, ensure pressed and released bit is cleared
			else {
				controllerBtnReleased[i] &= ~bitMask;
				controllerBtnPressed[i] &= ~bitMask;
			}
		}
	}
}

/************************
 * Controller
*************************/

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

/************************
 * Holding Button Down
*************************/

// Given a requested config, returns if the config matches the requested config exactly
u8 controllerBtnDownMatchConfig(int controller, int reqControllerBtnConfig)
{
	u16 data = controllerBtnDown[controller];
	return (data & reqControllerBtnConfig) == reqControllerBtnConfig;
}

// Given a requested config, returns if at least one controller is plugged into
// requested config
u8 controllerBtnDownAtLeastConfig(int controller, int reqControllerBtnConfig)
{
	u16 data = controllerBtnDown[controller];
	return (data & reqControllerBtnConfig) > 0;
}

// Returns if at least 1 controller is plugged in
u8 controllerBtnDownAtLeast1(int controller)
{
	u16 data = controllerBtnDown[controller];
	return data > 0;
}

u8 controllerBtnDownCount(int controller)
{
	int ret = 0;
	u16 data = controllerBtnDown[controller];

	u8 i = 0;
	for(i = 0; i < 16; i++)
	{
		u16 bit = 1 << i;
		if (bit & data)
			ret++;
	}

	return ret;
}

/************************
 * Pressed Button
*************************/

// Given a requested config, returns if the config matches the requested config exactly
u8 controllerBtnPressedMatchConfig(int controller, int reqControllerBtnConfig)
{
	u16 data = controllerBtnPressed[controller];
	return (data & reqControllerBtnConfig) == reqControllerBtnConfig;
}

// Given a requested config, returns if at least one controller is plugged into
// requested config
u8 controllerBtnPressedAtLeastConfig(int controller, int reqControllerBtnConfig)
{
	u16 data = controllerBtnPressed[controller];
	return (data & reqControllerBtnConfig) > 0;
}

// Returns if at least 1 controller is plugged in
u8 controllerBtnPressedAtLeast1(int controller)
{
	u16 data = controllerBtnPressed[controller];
	return data > 0;
}

u8 controllerBtnPressedCount(int controller)
{
	int ret = 0;
	u16 data = controllerBtnPressed[controller];

	u8 i = 0;
	for (i = 0; i < 16; i++)
	{
		u16 bit = 1 << i;
		if (bit & data)
			ret++;
	}

	return ret;
}

/************************
 * Released button
*************************/

// Given a requested config, returns if the config matches the requested config exactly
u8 controllerBtnReleasedMatchConfig(int controller, int reqControllerBtnConfig)
{
	u16 data = controllerBtnReleased[controller];
	return (data & reqControllerBtnConfig) == reqControllerBtnConfig;
}

// Given a requested config, returns if at least one controller is plugged into
// requested config
u8 controllerBtnReleasedAtLeastConfig(int controller, int reqControllerBtnConfig)
{
	u16 data = controllerBtnReleased[controller];
	return (data & reqControllerBtnConfig) > 0;
}

// Returns if at least 1 controller is plugged in
u8 controllerBtnReleasedAtLeast1(int controller)
{
	u16 data = controllerBtnReleased[controller];
	return data > 0;
}

u8 controllerBtnReleasedCount(int controller)
{
	int ret = 0;
	u16 data = controllerBtnReleased[controller];

	u8 i = 0;
	for (i = 0; i < 16; i++)
	{
		u16 bit = 1 << i;
		if (bit & data)
			ret++;
	}

	return ret;
}

/************************
 * Joystick
*************************/

// Gets data from the joystick as floats in the range of -1.00 to 1.00
// Takes into account deadzone and proper max ranges including angled ranges.
// Data is clamped
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
		retX = ((float)x) / ((float)CONTROLLER_JOY_RNG_X);
		retY = ((float)y) / ((float)CONTROLLER_JOY_RNG_Y);
	}
	else {
		retX = ((float)x) / ((float)CONTROLLER_JOY_RNG_ANG_X);
		retY = ((float)y) / ((float)CONTROLLER_JOY_RNG_ANG_Y);
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
