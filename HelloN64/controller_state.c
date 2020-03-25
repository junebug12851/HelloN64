#include <nusys.h>

NUContData controllerData[4];      // Player 1 Controller data
u8 controllerPattern;              // Controller Manager

void controllerInit()
{
	// Initialize the controllers
	controllerPattern = nuContInit();
}

void controllerRefreshButtonState()
{
	// Get controller input
	nuContDataGetExAll(controllerData);
}

// Returns a 1 or 0 based on requested button state
u8 controllerCheckButton(int btnState)
{
	return (controllerPattern & (1 << (btnState))) > 0;
}
