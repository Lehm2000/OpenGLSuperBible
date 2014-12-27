#ifndef INPUTACTION_H
#define INPUTACTION_H

/**
	InputAction
	Holds the key/mouse combination for an action.  These are held in an array in GEInputState
	Author: Jeff Adams
*/

#include <vector>

class InputAction
{
private:

	// Members
	std::vector<unsigned short> keys;			// list of keyboard keys that need to be pressed
	std::vector<unsigned char> mouseButtons;	// list of mouse buttons that need to be pressed
	// std::vector<unsigned char> joyButtons;	// list of joy buttons... if needed.

public:

	// Structors

	// ... not needed?

	// Getters
	
	unsigned short getKey( unsigned int keyIndex ) const; 
	unsigned char getNumKeys() const;
	unsigned short getMouseButton( unsigned int buttonIndex ) const; 
	unsigned char getNumMouseButtons() const;

	// Functions

	void AddKey( const unsigned short key );
	void ClearKeys();
	void AddMouseButton( const unsigned char mouseButton );
	void ClearMouseButtons();
	
};

#endif /* INPUTACTION_H */