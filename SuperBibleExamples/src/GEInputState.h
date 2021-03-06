#ifndef GEINPUTSTATE_H
#define GEINPUTSTATE_H

/**
	GEInputState class
	
	Purpose:  To hold the current state of the input devices.  Implemented as a
	GEObject so that it can be put in the gameEntities and accessible to 
	the controllers.

	Author: Jeff Adams
*/

#include <string>
#include <vector>
#include <glm\glm.hpp>

#include "GEObject.h"
#include "InputAction.h"
#include "TypeDefinitions.h"

#define INPUTSTATE_MAX_KEY_BUTTONS		512
#define INPUTSTATE_MAX_MOUSE_BUTTONS	8

// Input Action List.  This is the list of possible actions in the game/engine.  These will be used as the index of the ActionList

#define GE_MAX_INPUT_ACTIONS 256	// totally arbitrary number, can be revised upward if needed...

#define GE_ENGINE_ACTION_NONE					0
#define GE_ENGINE_ACTION_CHANGERENDERMODE		1
#define GE_ENGINE_ACTION_TOGGLEBOUNDINGBOX		2
#define GE_ENGINE_ACTION_CHANGECURSORMODE		3
#define GE_ENGINE_ACTION_TOGGLEMULTISAMPLE		4

// mouseModes
#define GE_MOUSEMODE_LOOK		0
#define GE_MOUSEMODE_POINT		1

class GEInputState
{
private:

	// Members
	bool keyboardKeys[ INPUTSTATE_MAX_KEY_BUTTONS ];
	bool mouseButtons[ INPUTSTATE_MAX_MOUSE_BUTTONS ];
	unsigned char mouseMode;							// What mode is the mouse? Member of GE_MOUSEMODE_*
	GEvec2 mousePosition;
	GEvec2 mousePositionPrev;  
	GEvec2 mouseScrollOffset;	// how much the mouse scrolled, this needs to be reset at the beginning of each frame.

	InputAction actionList[ GE_MAX_INPUT_ACTIONS ];		/* This holds the key/button combinations necessary to activate 
			actions. The index matches GE_ENGINE_ACTION_* */

	std::vector<std::string> mouseOverObjects;  /* list of objects the mouse is over.  Not technically an input state... but
		seemed to fit best here */

public:

	// Structors
	GEInputState();
	GEInputState( const GEInputState& source );
	GEInputState( GEvec2 mousePosition );

	// Setters
	void setKeyboardKey( const unsigned short key, bool pressed );
	void setMouseButton( const unsigned short button, bool pressed );
	void setMousePosition( const GEvec2 mousePosition );
	//void setMousePositionPrev( const GEvec2 mousePosition );
	void setMouseScrollOffset( const GEvec2 mouseScrollOffset );
	void setMouseMode( const unsigned char mouseMode );
	void setMouseOverObjects( const std::vector< std::string > mouseOverObjects);
	/**
		setInputAction
		Uses provided index ( GE_ENGINE_ACTION_* ) to set an InputAction in the actionList array.
	*/
	void setInputAction( const unsigned int index, const InputAction inputAction );

	// Getters
	bool getKeyboardKey( const unsigned short key ) const;
	bool getMouseButton( const unsigned short button ) const;
	GEvec2 getMousePosition( ) const;
	GEvec2 getMousePositionPrev( ) const;
	GEvec2 getMouseScrollOffset( ) const;
	unsigned char getMouseMode( ) const;
	unsigned char getMouseModePrev( ) const;
	std::vector< std::string > getMouseOverObjects() const;

	// Functions

	/**
	 * Sets the current mouse position and prev mouse posiiton to the specified position.  This allows you to move the
	 * mouse position without it registering as a mouse move.
	 */
	void ResetMousePosition( const GEvec2 mousePosition );
	
	/**
	 * Sets the mouse prev position to the mouse position.  Must be called at the beginning of each frame.
	 */
	void UpdateMousePrev();

	

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

	/**
		ActionToggled
		All the required buttons for an action are pressed
	*/
	bool ActionToggled( unsigned int actionIndex ) const;
};

#endif /* GEINPUTSTATE_H */