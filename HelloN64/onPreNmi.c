#include "tv.h"

// This is when the "Reset" button is pressed

void onPreNmi()
{
    // This calls the associated preNmi for the compiled TV format
    // if there is one
    tv_PreNmi();
}
