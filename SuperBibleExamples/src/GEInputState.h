#ifndef GEINPUTSTATE_H
#define GEINPUTSTATE_H

/**
	GEInputState class
	
	Purpose:  To hold the current state of the input devices.  Implemented as a
	GEObject so that it can be put in the gameEntities and accessible to 
	the controllers.

	Author: Jeff Adams
*/

#include <glm\glm.hpp>

#include "GEObject.h"
#include "TypeDefinitions.h"

#define INPUTSTATE_MAX_KEY_BUTTONS		512
#define INPUTSTATE_MAX_MOUSE_BUTTONS	8

class GEInputState: public GEObject
{
private:

	// Members
	bool keyboardKeys[ INPUTSTATE_MAX_KEY_BUTTONS ];
	bool mouseButtons[ INPUTSTATE_MAX_MOUSE_BUTTONS ];
	GEvec2 mousePosition;
	GEvec2 mousePositionPrev;  // Is this one necessary or will the controllers be responsible for tracking the previous position?
	GEvec2 mouseScrollOffset;	// how much the mouse scrolled, this needs to be reset at the beginning of each frame.

public:

	// Structors
	GEInputState();
	GEInputState( const GEInputState& source );
	GEInputState( GEvec2 mousePosition );

	// Setters
	void setKeyboardKey( const unsigned short key, bool pressed );
	void setMouseButton( const unsigned short button, bool pressed );
	void setMousePosition( const GEvec2 mousePosition );
	void setMouseScrollOffset( const GEvec2 mouseScrollOffset );  

	// Getters
	bool getKeyboardKey( const unsigned short key ) const;
	bool getMouseButton( const unsigned short button ) const;
	GEvec2 getMousePosition( ) const;
	GEvec2 getMouseScrollOffset( ) const;

	// Functions

	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual GEInputState* clone() const;

	virtual std::string getClassName() const;

	void ResetMouseScrollOffset();
	
	/**
		KeyToString()
		Takes a keyboard key index and returns the string equivelant. 
		@param keyIndex - which key to return string for
		@return - the string representation of the key
	*/
	std::string KeyToString( unsigned int keyIndex ) const;

	/**
		ButtonToString()
		Takes a mouse button index and returns the string equivelant. 
		@param buttonIndex - which key to return string for
		@return - the string representation of the key
	*/
	std::string ButtonToString( unsigned int buttonIndex ) const;
};

#endif /* GEINPUTSTATE_H */