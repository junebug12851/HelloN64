#ifndef FONT_TILEMAP_H
#define FONT_TILEMAP_H

#include <nusys.h>

// Tilemap has 4 banks due to memory restrictions

// Tile size in width and height
#define FONT_TILE_SIZE_PIXELS 8

// Tile size radix, basically the radix number used to
// grab the tile x and y coords from the tileID
#define FONT_TILE_SIZE_RADIX 0x10

// Total tilemap size in tiles (both banks combined)
#define FONT_TILEMAP_WIDTH_TILES 16
#define FONT_TILEMAP_HEIGHT_TILES 8

// Total tilemap size in pixels (both banks combined)
#define FONT_TILEMAP_WIDTH_PIXELS (FONT_TILEMAP_WIDTH_TILES * FONT_TILE_SIZE_PIXELS)
#define FONT_TILEMAP_HEIGHT_PIXELS (FONT_TILEMAP_HEIGHT_TILES * FONT_TILE_SIZE_PIXELS)

// Total tilemap tile count  (both banks combined)
#define FONT_TILE_COUNT (FONT_TILEMAP_WIDTH_TILES * FONT_TILEMAP_HEIGHT_TILES)

// Total tilemap array size with both banks combined
#define FONT_TILEMAP_ARR_SIZE ((FONT_TILEMAP_WIDTH_TILES * FONT_TILEMAP_HEIGHT_TILES * (FONT_TILE_SIZE_PIXELS * FONT_TILE_SIZE_PIXELS)) / 2)

// Single bank tilemap size in tiles
#define FONT_TILEMAP_PART_WIDTH_TILES FONT_TILEMAP_WIDTH_TILES
#define FONT_TILEMAP_PART_HEIGHT_TILES (FONT_TILEMAP_HEIGHT_TILES / 4)

// Single bank tilemap size in pixels
#define FONT_TILEMAP_PART_WIDTH_PIXELS (FONT_TILEMAP_PART_WIDTH_TILES * FONT_TILE_SIZE_PIXELS)
#define FONT_TILEMAP_PART_HEIGHT_PIXELS (FONT_TILEMAP_PART_HEIGHT_TILES * FONT_TILE_SIZE_PIXELS)

// Single bank tilemap tile count
#define FONT_TILE_PART_COUNT (FONT_TILEMAP_PART_WIDTH_TILES * FONT_TILEMAP_PART_HEIGHT_TILES)

// Single bank tilemap array size
#define FONT_TILEMAP_PART_ARR_SIZE ((FONT_TILEMAP_PART_WIDTH_TILES * FONT_TILEMAP_PART_HEIGHT_TILES * (FONT_TILE_SIZE_PIXELS * FONT_TILE_SIZE_PIXELS)) / 2)

// LUT size for both banks
#define FONT_TILEMAP_LUT_SIZE 4

// Access to the LUT
extern u16 font_tilemap_lut[FONT_TILEMAP_LUT_SIZE];

// Access to tilemap bank 0
// A-Z 0-5
extern u8 font_tilemap_1[FONT_TILEMAP_PART_ARR_SIZE];

// Access to tilemap bank 1
// 6-9 a-z . " "
extern u8 font_tilemap_2[FONT_TILEMAP_PART_ARR_SIZE];

// Access to tilemap bank 2
// Symbols 1, mostly extended ascii
extern u8 font_tilemap_3[FONT_TILEMAP_PART_ARR_SIZE];

// Access to tilemap bank 3
// Symbols 2, mostly ascii-art symbols
extern u8 font_tilemap_4[FONT_TILEMAP_PART_ARR_SIZE];

// With both banks combined, these are all the tiles offered
enum {
	// Capital Letters
	FONT_UPPER_A = 0,
	FONT_UPPER_B,
	FONT_UPPER_C,
	FONT_UPPER_D,
	FONT_UPPER_E,
	FONT_UPPER_F,
	FONT_UPPER_G,
	FONT_UPPER_H,
	FONT_UPPER_I,
	FONT_UPPER_J,
	FONT_UPPER_K,
	FONT_UPPER_L,
	FONT_UPPER_M,
	FONT_UPPER_N,
	FONT_UPPER_O,
	FONT_UPPER_P,
	FONT_UPPER_Q,
	FONT_UPPER_R,
	FONT_UPPER_S,
	FONT_UPPER_T,
	FONT_UPPER_U,
	FONT_UPPER_V,
	FONT_UPPER_W,
	FONT_UPPER_X,
	FONT_UPPER_Y,
	FONT_UPPER_Z,

	// Numbers
	FONT_NUMBER_0,
	FONT_NUMBER_1,
	FONT_NUMBER_2,
	FONT_NUMBER_3,
	FONT_NUMBER_4,
	FONT_NUMBER_5,
	FONT_NUMBER_6,
	FONT_NUMBER_7,
	FONT_NUMBER_8,
	FONT_NUMBER_9,

	// Lowercase Letters
	FONT_LOWER_A,
	FONT_LOWER_B,
	FONT_LOWER_C,
	FONT_LOWER_D,
	FONT_LOWER_E,
	FONT_LOWER_F,
	FONT_LOWER_G,
	FONT_LOWER_H,
	FONT_LOWER_I,
	FONT_LOWER_J,
	FONT_LOWER_K,
	FONT_LOWER_L,
	FONT_LOWER_M,
	FONT_LOWER_N,
	FONT_LOWER_O,
	FONT_LOWER_P,
	FONT_LOWER_Q,
	FONT_LOWER_R,
	FONT_LOWER_S,
	FONT_LOWER_T,
	FONT_LOWER_U,
	FONT_LOWER_V,
	FONT_LOWER_W,
	FONT_LOWER_X,
	FONT_LOWER_Y,
	FONT_LOWER_Z,

	// Symbols and Misc
	FONT_PERIOD,
	FONT_SPACE,

	// We're starting Bank 2 here
	FONT_EXCLAMATION,
	FONT_OPEN_DQUOTE,
	FONT_CLOSE_DQUOTE,
	FONT_HASHTAG,
	FONT_DOLLAR,
	FONT_PERCENT,
	FONT_AMPERSAND,
	FONT_OPEN_SQUOTE,
	FONT_CLOSE_SQUOTE,
	FONT_DQUOTE,
	FONT_SQUOTE,
	FONT_OPAREN,
	FONT_CPAREN,
	FONT_ASTERISK,
	FONT_PLUS,
	FONT_COMMA,
	FONT_MINUS,
	FONT_FSLASH,
	FONT_COLON,
	FONT_SEMI_COLON,
	FONT_ANGLED_OBRACKET,
	FONT_EQUAL,
	FONT_ANGLED_CBRACKET,
	FONT_QUESTION,
	FONT_AT_SYMBOL,
	FONT_OSQ_BRACKET,
	FONT_CSQ_BRACKET,
	FONT_BACKSLASH,
	FONT_CARET,
	FONT_UNDERSCORE,
	FONT_MDOT,
	FONT_PIPE,
	FONT_OCURLY,
	FONT_CCURLY,
	FONT_TILDE,

	// Ascii-Art
	FONT_DITHER_LIGHT,
	FONT_DITHER_MEDIUM,
	FONT_DITHER_HEAVY,

	// Lines/Double Lines
	// TRBL | TBRL (Tob, Right, Bottom, Left) | (Top, Bottom) (Right, Left)
	FONT_SLINE_TB,
	FONT_SLINE_TBL,
	FONT_DLINE_TBL,
	FONT_DLINE_TB,
	FONT_DLINE_TR,
	FONT_DLINE_BR,
	FONT_SLINE_TR,
	FONT_SLINE_BL,
	FONT_SLINE_TRL,
	FONT_SLINE_RBL,
	FONT_SLINE_TRB,
	FONT_SLINE_LR,
	FONT_SLINE_TRBL,
	FONT_DLINE_BL,
	FONT_DLINE_TL,
	FONT_DLINE_TRL,
	FONT_DLINE_RBL,
	FONT_DLINE_TRB,
	FONT_DLINE_LR,
	FONT_DLINE_TRBL,
	FONT_SLINE_BR,
	FONT_SLINE_TL,

	// Misc
	FONT_BOX_HEAVY,
	FONT_BOX_MEDIUM,
	FONT_BOX_LIGHT,
	FONT_PIPE_B // Don't know what to call this one
};

#endif
