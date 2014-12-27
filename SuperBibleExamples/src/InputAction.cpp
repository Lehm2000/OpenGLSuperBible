#include "InputAction.h"

// Structors

// ... not needed?

// Getters
	
unsigned short InputAction::getKey( unsigned int keyIndex ) const
{
	return keys[keyIndex];
}

unsigned char InputAction::getNumKeys() const
{
	return keys.size();
}

unsigned short InputAction::getMouseButton( unsigned int buttonIndex ) const
{
	return mouseButtons[ buttonIndex ];
}
 
unsigned char InputAction::getNumMouseButtons() const
{
	return mouseButtons.size();
}


// Functions

void InputAction::AddKey( const unsigned short key )
{
	keys.push_back( key );
}

void InputAction::ClearKeys()
{
	keys.clear();
}

void InputAction::AddMouseButton( const unsigned char mouseButton )
{
	mouseButtons.push_back( mouseButton );
}

void InputAction::ClearMouseButtons()
{
	mouseButtons.clear();
}
