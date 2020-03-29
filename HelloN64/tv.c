#include <nusys.h>
#include "tv.h"

void tvPreNmi()
{
	switch (osTvType)
	{
	case OS_TV_NTSC:
		return tvNtscPreNmi();
	case OS_TV_MPAL:
		return tvMpalPreNmi();
	case OS_TV_PAL:
		return tvPalPreNmi();
	}
}

void tvInit()
{
	switch (osTvType)
	{
	case OS_TV_NTSC:
		return tvNtscInit();
	case OS_TV_MPAL:
		return tvMpalInit();
	case OS_TV_PAL:
		return tvPalInit();
	}
}
