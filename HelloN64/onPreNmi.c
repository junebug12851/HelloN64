#include "hack_tvFormat.h"

// This is when the "Reset" button is pressed

void onPreNmi()
{
    // This calls the associated hack for the compiled TV format
    // if there is one
    hack_tvPreNmi();
}
