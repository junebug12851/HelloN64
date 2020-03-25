#include <nusys.h>
#include "gfx_materialColor.h"

struct ColorRGB materialColorList[MATERIAL_COLOR_COUNT] = {
	{255,  82,  82}, //  0) Red
	{255,  64, 129}, //  1) Pink
	{224,  64, 251}, //  2) Purple
	{124,  77, 255}, //  3) Deep Purple
	{ 83, 109, 254}, //  4) Indigo
	{ 68, 138, 255}, //  5) Blue
	{  3, 169, 244}, //  6) Light Blue
	{  0, 188, 212}, //  7) Cyan
	{  0, 150, 136}, //  8) Teal
	{ 76, 175,  80}, //  9) Green
	{139, 195,  74}, // 10) Light Green
	{205, 220,  57}, // 11) Lime
	{255, 235,  59}, // 12) Yellow
	{255, 193,   7}, // 13) Amber
	{255, 152,   0}, // 14) Orange
	{255,  87,  34}, // 15) Deep Orange
	{121,  85,  72}, // 16) Brown
	{158, 158, 158}, // 17) Grey
	{ 69,  90, 100}, // 18) Dark Blue Grey
};
