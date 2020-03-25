#include <nusys.h>

NUContData contdata[4];      // Player 1 Controller data
u8 contPattern;              // Controller Manager

void controller_init()
{
	// Initialize the controllers
	contPattern = nuContInit();
}

void controller_refreshButtonState()
{
	// Get controller input
	nuContDataGetExAll(contdata);
}

// Returns a 1 or 0 based on requested button state
u8 controller_checkButton(int btnState)
{
	return (contPattern & (1 << (btnState))) > 0;
}
