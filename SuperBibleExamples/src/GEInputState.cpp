
#include <glm\glm.hpp>

#include "GEInputState.h"
#include "InputItem.h"

// Structors
GEInputState::GEInputState()
{
	// initialize the inputstate members

	for ( unsigned short i = 0; i< INPUTSTATE_MAX_KEY_BUTTONS; i++ )
	{
		this->setKeyboardKey( i, false );
	}

	for ( unsigned short i = 0; i< INPUTSTATE_MAX_MOUSE_BUTTONS; i++ )
	{
		this->setMouseButton( i, false );
	}

	this->setMousePosition( glm::vec2( 0.0f, 0.0f ) );

	// set other parameters
	this->setVisible( false );	// this is not a visible game object.
}

// Setters
void GEInputState::setKeyboardKey( const unsigned short key, bool pressed )
{
	this->keyboardKeys[key] = pressed;
}

void GEInputState::setMouseButton( const unsigned short button, bool pressed )
{
	this->mouseButtons[button] = pressed;
}

void GEInputState::setMousePosition( const glm::vec2 mousePosition )
{
	this->mousePosition = mousePosition;
}

// mouseScroll?

// Getters
bool GEInputState::getKeyboardKey( const unsigned short key ) const
{
	return this->keyboardKeys[ key ];
}

bool GEInputState::getMouseButton( const unsigned short button ) const
{
	return this->mouseButtons[ button ];
}

glm::vec2 GEInputState::getMousePosition( ) const
{
	return this->mousePosition;
}

// mouseScroll?

// Functions
std::string GEInputState::getClassName()
{
	return "GEInputState";
}

std::string GEInputState::KeyToString( unsigned int keyIndex )
{
	std::string returnString = "";

	switch ( keyIndex )
	{
	case GE_KEY_SPACE:
		returnString = " ";
		break;
	case GE_KEY_APOSTROPHE:
		returnString = "'";
		break;
	case GE_KEY_COMMA:
		returnString = ",";
		break;
	case GE_KEY_MINUS:
		returnString = "-";
		break;
	case GE_KEY_PERIOD:
		returnString = ".";
		break;
	case GE_KEY_SLASH:
		returnString = "/";
		break;
	case GE_KEY_0:
		returnString = "0";
		break;
	case GE_KEY_1:
		returnString = "1";
		break;
	case GE_KEY_2:
		returnString = "2";
		break;
	case GE_KEY_3:
		returnString = "3";
		break;
	case GE_KEY_4:
		returnString = "4";
		break;
	case GE_KEY_5:
		returnString = "5";
		break;
	case GE_KEY_6:
		returnString = "6";
		break;
	case GE_KEY_7:
		returnString = "7";
		break;
	case GE_KEY_8:
		returnString = "8";
		break;
	case GE_KEY_9:
		returnString = "9";
		break;
	case GE_KEY_SEMICOLON:
		returnString = ";";
		break;
	case GE_KEY_EQUAL:
		returnString = "=";
		break;
	case GE_KEY_A:
		returnString = "A";
		break;
	case GE_KEY_B:
		returnString = "B";
		break;
	case GE_KEY_C:
		returnString = "C";
		break;
	case GE_KEY_D:
		returnString = "D";
		break;
	case GE_KEY_E:
		returnString = "E";
		break;
	case GE_KEY_F:
		returnString = "F";
		break;
	case GE_KEY_G:
		returnString = "G";
		break;
	case GE_KEY_H:
		returnString = "H";
		break;
	case GE_KEY_I:
		returnString = "I";
		break;
	case GE_KEY_J:
		returnString = "J";
		break;
	case GE_KEY_K:
		returnString = "K";
		break;
	case GE_KEY_L:
		returnString = "L";
		break;
	case GE_KEY_M:
		returnString = "M";
		break;
	case GE_KEY_N:
		returnString = "N";
		break;
	case GE_KEY_O:
		returnString = "O";
		break;
	case GE_KEY_P:
		returnString = "P";
		break;
	case GE_KEY_Q:
		returnString = "Q";
		break;
	case GE_KEY_R:
		returnString = "R";
		break;
	case GE_KEY_S:
		returnString = "S";
		break;
	case GE_KEY_T:
		returnString = "T";
		break;
	case GE_KEY_U:
		returnString = "U";
		break;
	case GE_KEY_V:
		returnString = "V";
		break;
	case GE_KEY_W:
		returnString = "W";
		break;
	case GE_KEY_X:
		returnString = "X";
		break;
	case GE_KEY_Y:
		returnString = "Y";
		break;
	case GE_KEY_Z:
		returnString = "Z";
		break;
	case GE_KEY_LEFT_BRACKET:
		returnString = "[";
		break;
	case GE_KEY_BACKSLASH:
		returnString = "\\";
		break;
	case GE_KEY_RIGHT_BRACKET:
		returnString = "]";
		break;
	case GE_KEY_GRAVE_ACCENT:
		returnString = "`";
		break;
	case GE_KEY_WORLD_1:
		returnString = "#";
		break;
	case GE_KEY_WORLD_2:
		returnString = "#";
		break;
	
	}
	

	/* Function keys */
	/*#define GE_KEY_ESCAPE             256
	#define GE_KEY_ENTER              257
	#define GE_KEY_TAB                258
	#define GE_KEY_BACKSPACE          259
	#define GE_KEY_INSERT             260
	#define GE_KEY_DELETE             261
	#define GE_KEY_RIGHT              262
	#define GE_KEY_LEFT               263
	#define GE_KEY_DOWN               264
	#define GE_KEY_UP                 265
	#define GE_KEY_PAGE_UP            266
	#define GE_KEY_PAGE_DOWN          267
	#define GE_KEY_HOME               268
	#define GE_KEY_END                269
	#define GE_KEY_CAPS_LOCK          280
	#define GE_KEY_SCROLL_LOCK        281
	#define GE_KEY_NUM_LOCK           282
	#define GE_KEY_PRINT_SCREEN       283
	#define GE_KEY_PAUSE              284
	#define GE_KEY_F1                 290
	#define GE_KEY_F2                 291
	#define GE_KEY_F3                 292
	#define GE_KEY_F4                 293
	#define GE_KEY_F5                 294
	#define GE_KEY_F6                 295
	#define GE_KEY_F7                 296
	#define GE_KEY_F8                 297
	#define GE_KEY_F9                 298
	#define GE_KEY_F10                299
	#define GE_KEY_F11                300
	#define GE_KEY_F12                301
	#define GE_KEY_F13                302
	#define GE_KEY_F14                303
	#define GE_KEY_F15                304
	#define GE_KEY_F16                305
	#define GE_KEY_F17                306
	#define GE_KEY_F18                307
	#define GE_KEY_F19                308
	#define GE_KEY_F20                309
	#define GE_KEY_F21                310
	#define GE_KEY_F22                311
	#define GE_KEY_F23                312
	#define GE_KEY_F24                313
	#define GE_KEY_F25                314
	#define GE_KEY_KP_0               320
	#define GE_KEY_KP_1               321
	#define GE_KEY_KP_2               322
	#define GE_KEY_KP_3               323
	#define GE_KEY_KP_4               324
	#define GE_KEY_KP_5               325
	#define GE_KEY_KP_6               326
	#define GE_KEY_KP_7               327
	#define GE_KEY_KP_8               328
	#define GE_KEY_KP_9               329
	#define GE_KEY_KP_DECIMAL         330
	#define GE_KEY_KP_DIVIDE          331
	#define GE_KEY_KP_MULTIPLY        332
	#define GE_KEY_KP_SUBTRACT        333
	#define GE_KEY_KP_ADD             334
	#define GE_KEY_KP_ENTER           335
	#define GE_KEY_KP_EQUAL           336
	#define GE_KEY_LEFT_SHIFT         340
	#define GE_KEY_LEFT_CONTROL       341
	#define GE_KEY_LEFT_ALT           342
	#define GE_KEY_LEFT_SUPER         343
	#define GE_KEY_RIGHT_SHIFT        344
	#define GE_KEY_RIGHT_CONTROL      345
	#define GE_KEY_RIGHT_ALT          346
	#define GE_KEY_RIGHT_SUPER        347
	#define GE_KEY_MENU               348
	#define GE_KEY_LAST               GE_KEY_MENU */

	return returnString;

}