
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

	this->setMousePosition( GEvec2( 0.0f, 0.0f ) );

	// set other parameters
	this->setVisible( false );	// this is not a visible game object.
}

GEInputState::GEInputState( GEvec2 mousePosition )
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

	this->setMousePosition( mousePosition );

	// set other parameters
	this->setVisible( false );	// this is not a visible game object.
}

GEInputState::GEInputState( const GEInputState& source )
	:GEObject( source )
{
	for ( unsigned short i = 0; i< INPUTSTATE_MAX_KEY_BUTTONS; i++ )
	{
		this->setKeyboardKey( i, source.keyboardKeys[i] );
	}

	for ( unsigned short i = 0; i< INPUTSTATE_MAX_MOUSE_BUTTONS; i++ )
	{
		this->setMouseButton( i, source.mouseButtons[i] );
	}

	this->setMousePosition( source.mousePosition );
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

void GEInputState::setMousePosition( const GEvec2 mousePosition )
{
	this->mousePosition = mousePosition;
}

void GEInputState::setMouseScrollOffset( const GEvec2 mouseScrollOffset )
{
	this->mouseScrollOffset +=  mouseScrollOffset;
}



// Getters
bool GEInputState::getKeyboardKey( const unsigned short key ) const
{
	return this->keyboardKeys[ key ];
}

bool GEInputState::getMouseButton( const unsigned short button ) const
{
	return this->mouseButtons[ button ];
}

GEvec2 GEInputState::getMousePosition( ) const
{
	return this->mousePosition;
}

GEvec2 GEInputState::getMouseScrollOffset( ) const
{
	return this->mouseScrollOffset;
}



// Functions

GEInputState* GEInputState::clone() const
{
	return new GEInputState(*this);
}

std::string GEInputState::getClassName() const
{
	return "GEInputState";
}

void GEInputState::ResetMouseScrollOffset()
{
	this->mouseScrollOffset = GEvec2( 0.0f, 0.0f );
}

std::string GEInputState::KeyToString( unsigned int keyIndex ) const
{
	std::string returnString = "";

	switch ( keyIndex )
	{
	case GE_KEY_SPACE:
		returnString = "SPACE";
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

		
	case GE_KEY_ESCAPE:
		returnString = "ESC";
		break;
	case GE_KEY_ENTER:
		returnString = "ENTER";
		break;
	case GE_KEY_TAB:
		returnString = "TAB";
		break;
	case GE_KEY_BACKSPACE:
		returnString = "BACKSPACE";
		break;
	case GE_KEY_INSERT:
		returnString = "INS";
		break;
	case GE_KEY_DELETE:
		returnString = "DEL";
		break;
	case GE_KEY_RIGHT:
		returnString = "RIGHT";
		break;
	case GE_KEY_LEFT:
		returnString = "LEFT";
		break;
	case GE_KEY_DOWN:
		returnString = "DOWN";
		break;
	case GE_KEY_UP:
		returnString = "UP";
		break;
	case GE_KEY_PAGE_UP:
		returnString = "PAGEUP";
		break;
	case GE_KEY_PAGE_DOWN:
		returnString = "PAGEDOWN";
		break;
	case GE_KEY_HOME:
		returnString = "HOME";
		break;
	case GE_KEY_END:
		returnString = "END";
		break;
	case GE_KEY_CAPS_LOCK:
		returnString = "CAPSLOCK";
		break;
	case GE_KEY_SCROLL_LOCK:
		returnString = "SCROLLLOCK";
		break;
	case GE_KEY_NUM_LOCK:
		returnString = "NUMLOCK";
		break;
	case GE_KEY_PRINT_SCREEN:
		returnString = "PRTSCRN";
		break;
	case GE_KEY_PAUSE:
		returnString = "PAUSE";
		break;
	case GE_KEY_F1:
		returnString = "F1";
		break;
	case GE_KEY_F2:
		returnString = "F2";
		break;
	case GE_KEY_F3:
		returnString = "F3";
		break;
	case GE_KEY_F4:
		returnString = "F4";
		break;
	case GE_KEY_F5:
		returnString = "F5";
		break;
	case GE_KEY_F6:
		returnString = "F6";
		break;
	case GE_KEY_F7:
		returnString = "F7";
		break;
	case GE_KEY_F8:
		returnString = "F8";
		break;
	case GE_KEY_F9:
		returnString = "F9";
		break;
	case GE_KEY_F10:
		returnString = "F10";
		break;
	case GE_KEY_F11:
		returnString = "F11";
		break;
	case GE_KEY_F12:
		returnString = "F12";
		break;
	case GE_KEY_F13:
		returnString = "F13";
		break;
	case GE_KEY_F14:
		returnString = "F14";
		break;
	case GE_KEY_F15:
		returnString = "F15";
		break;
	case GE_KEY_F16:
		returnString = "F16";
		break;
	case GE_KEY_F17:
		returnString = "F17";
		break;
	case GE_KEY_F18:
		returnString = "F18";
		break;
	case GE_KEY_F19:
		returnString = "F19";
		break;
	case GE_KEY_F20:
		returnString = "F20";
		break;
	case GE_KEY_F21:
		returnString = "F21";
		break;
	case GE_KEY_F22:
		returnString = "F22";
		break;
	case GE_KEY_F23:
		returnString = "F23";
		break;
	case GE_KEY_F24:
		returnString = "F24";
		break;
	case GE_KEY_F25:
		returnString = "F25";
		break;
	case GE_KEY_KP_0:
		returnString = "KP0";
		break;
	case GE_KEY_KP_1:
		returnString = "KP1";
		break;
	case GE_KEY_KP_2:
		returnString = "KP2";
		break;
	case GE_KEY_KP_3:
		returnString = "KP3";
		break;
	case GE_KEY_KP_4:
		returnString = "KP4";
		break;
	case GE_KEY_KP_5:
		returnString = "KP5";
		break;
	case GE_KEY_KP_6:
		returnString = "KP6";
		break;
	case GE_KEY_KP_7:
		returnString = "KP7";
		break;
	case GE_KEY_KP_8:
		returnString = "KP8";
		break;
	case GE_KEY_KP_9:
		returnString = "KP9";
		break;
	case GE_KEY_KP_DECIMAL:
		returnString = ".";
		break;
	case GE_KEY_KP_DIVIDE:
		returnString = "/";
		break;
	case GE_KEY_KP_MULTIPLY:
		returnString = "*";
		break;
	case GE_KEY_KP_SUBTRACT:
		returnString = "-";
		break;
	case GE_KEY_KP_ADD:
		returnString = "+";
		break;
	case GE_KEY_KP_ENTER:
		returnString = "ENTER";
		break;
	case GE_KEY_KP_EQUAL:
		returnString = "=";
		break;
	case GE_KEY_LEFT_SHIFT:
		returnString = "LFSHIFT";
		break;
	case GE_KEY_LEFT_CONTROL:
		returnString = "LFCTRL";
		break;
	case GE_KEY_LEFT_ALT:
		returnString = "LFALT";
		break;
	case GE_KEY_LEFT_SUPER:
		returnString = "LFSUPER";
		break;
	case GE_KEY_RIGHT_SHIFT:
		returnString = "RTSHIFT";
		break;
	case GE_KEY_RIGHT_CONTROL:
		returnString = "RTCTRL";
		break;
	case GE_KEY_RIGHT_ALT:
		returnString = "RTALT";
		break;
	case GE_KEY_RIGHT_SUPER:
		returnString = "RTSUPER";
		break;
	case GE_KEY_MENU:
		returnString = "MENU";
		break;
	
	}

	return returnString;

}

std::string GEInputState::ButtonToString( unsigned int buttonIndex ) const
{
	std::string returnString = "";

	switch ( buttonIndex )
	{
	case GE_MOUSE_BUTTON_1:
		returnString = "MB1";
		break;
	case GE_MOUSE_BUTTON_2:
		returnString = "MB2";
		break;
	case GE_MOUSE_BUTTON_3:
		returnString = "MB3";
		break;
	case GE_MOUSE_BUTTON_4:
		returnString = "MB4";
		break;
	case GE_MOUSE_BUTTON_5:
		returnString = "MB5";
		break;
	case GE_MOUSE_BUTTON_6:
		returnString = "MB6";
		break;
	case GE_MOUSE_BUTTON_7:
		returnString = "MB7";
		break;
	case GE_MOUSE_BUTTON_8:
		returnString = "MB8";
		break;
	}

	return returnString;
}