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

#define INPUTSTATE_MAX_KEY_BUTTONS		512
#define INPUTSTATE_MAX_MOUSE_BUTTONS	8

class GEInputState: public GEObject
{
private:

	// Members
	bool keyboardKeys[ INPUTSTATE_MAX_KEY_BUTTONS ];
	bool mouseButtons[ INPUTSTATE_MAX_MOUSE_BUTTONS ];
	glm::vec2 mousePosition;
	glm::vec2 mousePositionPrev;  // Is this one necessary or will the controllers be responsible for tracking the previous position?
	glm::vec2 mouseScroll;

public:

	// Structors
	GEInputState();

	// Setters
	void setKeyboardKey( const unsigned short key, bool pressed );
	void setMouseButton( const unsigned short button, bool pressed );
	void setMousePosition( const glm::vec2 mousePosition );
	// mouseScroll?

	// Getters
	bool getKeyboardKey( const unsigned short key ) const;
	bool getMouseButton( const unsigned short button ) const;
	glm::vec2 getMousePosition( ) const;
	// mouseScroll?

	// Functions
	virtual std::string getClassName();
	
	/**
		KeyToString()
		Takes a keyboard key index and returns the string equivelant. 
		@param keyIndex - which key to return string for
		@return - the string representation of the key
	*/
	std::string KeyToString( unsigned int keyIndex );

};

#endif /* GEINPUTSTATE_H */