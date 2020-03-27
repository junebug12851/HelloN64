#include <nusys.h>

#include "font_tilemap.h"
#include "font_tilemap_conversion.h"

u8 fontTilemapConvCache[FONT_TILEMAP_CONV_CACHE_SIZE];

u8 fontTilemapConvertChar(u8 c)
{
	// These (0-9)
	// NULL, SOH, STX, ETX, EOT, ENQ, ACK, BEL, BS, TAB
	// will be replaced by the tiles for the actual numbers 0-9
	// The rest will be a space unless it corresponds to an actual tile

	// If it's not a typical ascii character it cannot go through this function
	// If it does it will print out a space instead for an invalid character.
	// To use a non-ascii character you must manually use it via it's Tile ID or Name Alias

	switch (c)
	{
	case 0:
		return FONT_NUMBER_0;
	case 1:
		return FONT_NUMBER_1;
	case 2:
		return FONT_NUMBER_2;
	case 3:
		return FONT_NUMBER_3;
	case 4:
		return FONT_NUMBER_4;
	case 5:
		return FONT_NUMBER_5;
	case 6:
		return FONT_NUMBER_6;
	case 7:
		return FONT_NUMBER_7;
	case 8:
		return FONT_NUMBER_8;
	case 9:
		return FONT_NUMBER_9;
	case 'A':
		return FONT_UPPER_A;
	case 'B':
		return FONT_UPPER_B;
	case 'C':
		return FONT_UPPER_C;
	case 'D':
		return FONT_UPPER_D;
	case 'E':
		return FONT_UPPER_E;
	case 'F':
		return FONT_UPPER_F;
	case 'G':
		return FONT_UPPER_G;
	case 'H':
		return FONT_UPPER_H;
	case 'I':
		return FONT_UPPER_I;
	case 'J':
		return FONT_UPPER_J;
	case 'K':
		return FONT_UPPER_K;
	case 'L':
		return FONT_UPPER_L;
	case 'M':
		return FONT_UPPER_M;
	case 'N':
		return FONT_UPPER_N;
	case 'O':
		return FONT_UPPER_O;
	case 'P':
		return FONT_UPPER_P;
	case 'Q':
		return FONT_UPPER_Q;
	case 'R':
		return FONT_UPPER_R;
	case 'S':
		return FONT_UPPER_S;
	case 'T':
		return FONT_UPPER_T;
	case 'U':
		return FONT_UPPER_U;
	case 'V':
		return FONT_UPPER_V;
	case 'W':
		return FONT_UPPER_W;
	case 'X':
		return FONT_UPPER_X;
	case 'Y':
		return FONT_UPPER_Y;
	case 'Z':
		return FONT_UPPER_Z;
	case '0':
		return FONT_NUMBER_0;
	case '1':
		return FONT_NUMBER_1;
	case '2':
		return FONT_NUMBER_2;
	case '3':
		return FONT_NUMBER_3;
	case '4':
		return FONT_NUMBER_4;
	case '5':
		return FONT_NUMBER_5;
	case '6':
		return FONT_NUMBER_6;
	case '7':
		return FONT_NUMBER_7;
	case '8':
		return FONT_NUMBER_8;
	case '9':
		return FONT_NUMBER_9;
	case 'a':
		return FONT_LOWER_A;
	case 'b':
		return FONT_LOWER_B;
	case 'c':
		return FONT_LOWER_C;
	case 'd':
		return FONT_LOWER_D;
	case 'e':
		return FONT_LOWER_E;
	case 'f':
		return FONT_LOWER_F;
	case 'g':
		return FONT_LOWER_G;
	case 'h':
		return FONT_LOWER_H;
	case 'i':
		return FONT_LOWER_I;
	case 'j':
		return FONT_LOWER_J;
	case 'k':
		return FONT_LOWER_K;
	case 'l':
		return FONT_LOWER_L;
	case 'm':
		return FONT_LOWER_M;
	case 'n':
		return FONT_LOWER_N;
	case 'o':
		return FONT_LOWER_O;
	case 'p':
		return FONT_LOWER_P;
	case 'q':
		return FONT_LOWER_Q;
	case 'r':
		return FONT_LOWER_R;
	case 's':
		return FONT_LOWER_S;
	case 't':
		return FONT_LOWER_T;
	case 'u':
		return FONT_LOWER_U;
	case 'v':
		return FONT_LOWER_V;
	case 'w':
		return FONT_LOWER_W;
	case 'x':
		return FONT_LOWER_X;
	case 'y':
		return FONT_LOWER_Y;
	case 'z':
		return FONT_LOWER_Z;
	case '.':
		return FONT_PERIOD;
	case ' ':
		return FONT_SPACE;
	case '!':
		return FONT_EXCLAMATION;
	case '#':
		return FONT_HASHTAG;
	case '$':
		return FONT_DOLLAR;
	case '%':
		return FONT_PERCENT;
	case '&':
		return FONT_AMPERSAND;
	case '"':
		return FONT_DQUOTE;
	case '\'':
		return FONT_SQUOTE;
	case '(':
		return FONT_OPAREN;
	case ')':
		return FONT_CPAREN;
	case '*':
		return FONT_ASTERISK;
	case '+':
		return FONT_PLUS;
	case ',':
		return FONT_COMMA;
	case '-':
		return FONT_MINUS;
	case '/':
		return FONT_FSLASH;
	case ':':
		return FONT_COLON;
	case ';':
		return FONT_SEMI_COLON;
	case '<':
		return FONT_ANGLED_OBRACKET;
	case '=':
		return FONT_EQUAL;
	case '>':
		return FONT_ANGLED_CBRACKET;
	case '?':
		return FONT_QUESTION;
	case '@':
		return FONT_AT_SYMBOL;
	case '[':
		return FONT_OSQ_BRACKET;
	case ']':
		return FONT_CSQ_BRACKET;
	case '\\':
		return FONT_BACKSLASH;
	case '^':
		return FONT_CARET;
	case '_':
		return FONT_UNDERSCORE;
	case '|':
		return FONT_PIPE;
	case '{':
		return FONT_OCURLY;
	case '}':
		return FONT_CCURLY;
	case '~':
		return FONT_TILDE;
	}

	// Invalid, display nothing
	return FONT_SPACE;
}

void fontTilemapConvCacheInit()
{
	u8 i;
	for (i = 0; i < FONT_TILEMAP_CONV_CACHE_SIZE; i++)
	{
		fontTilemapConvCache[i] = fontTilemapConvertChar(i);
	}
}

// Because strings are null-terminated, this can't convert raw numbers, only character numbers
// This outputs tile ids to an array when given a string
u8 convertString(char* str, u8* out)
{
	// Loop until you reach a null terminator
	char c = *str;
	while (c != 0)
	{
		*out = fontTilemapConvCache[c];

		str++;
		out++;
		c = *str;
	}
}
