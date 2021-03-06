#ifndef GECONTROLLERINPUTKEY_H
#define GECONTROLLERINPUTKEY_H

/**
	GEControllerInputKey
	Purpose: GEController that monitors the status of a specified key and applies the specifed deltaVec to it.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"
#include "GEInputState.h"
#include "TypeDefinitions.h"

template <class T>
class GEControllerInputKey: public GEControllerConstant<T>
{
private:
	unsigned short key;		// key to listen for.
	bool pressed;			// is the key pressed
	bool pressedPrev;		// was the key pressed the last time around.

public:
	GEControllerInputKey();
	GEControllerInputKey( const T valueDelta, unsigned short key );
	GEControllerInputKey( const GEControllerInputKey<T>& source );
	virtual ~GEControllerInputKey();

	// Setters
	void setKey( unsigned short key );

	// Getters
	unsigned int getKey() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerInputKey<T>* clone() const;

	/**
		Control()
		Takes objectVector and applies the deltaVec to it if this objects key is pressed.
		@param startVector - starting point of the object.  Could be position, rotation or scale
		@param gameTime - time since the game started
		@param deltaTime - time since the last frame
		@return
	*/
	virtual T Control( const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin );

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual T CalcTransform( const T sourceValue );

	/**
		ProcessInput
		Function for processing input from the user.  Meant to be stored in the inputFunction list as a pointer.
	*/
	virtual void ProcessInput( const GEInputState* inputState );
};

template <class T>
GEControllerInputKey<T>::GEControllerInputKey()
	:GEControllerConstant<T>()
{
	this->setKey( GE_KEY_UNKNOWN );
	this->pressed = false;
	this->pressedPrev = false;
	
}

template <class T>
GEControllerInputKey<T>::GEControllerInputKey( const T valueDelta, unsigned short key )
	:GEControllerConstant<T>( valueDelta )
{
	this->setKey( key );
	this->pressed = false;
	this->pressedPrev = false;
}

template <class T>
GEControllerInputKey<T>::GEControllerInputKey( const GEControllerInputKey<T>& source )
	:GEControllerConstant<T>( source.valueDelta )
{
	this->setKey( source.key );
	this->pressed = false;
	this->pressedPrev = false;
}

template <class T>
GEControllerInputKey<T>::~GEControllerInputKey()
{
}


// Setters

template <class T>
void GEControllerInputKey<T>::setKey( unsigned short key )
{
	this->key = key;
}


// Getters

template <class T>
unsigned int GEControllerInputKey<T>::getKey() const
{
	return this->key;
}


// Functions

template <class T>
GEControllerInputKey<T>* GEControllerInputKey<T>::clone() const
{
	return new GEControllerInputKey<T>( *this );
}

template <class T>
T GEControllerInputKey<T>::Control( const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin )
{
	

	// then apply the delta if the key is pressed.
	if ( this->pressed )
	{
		transformedValue = transformedValue + ( valueDelta * (float)deltaTime );
	}

	transformedValue = ValidateRange( transformedValue, prevValue, max, useMax, min, useMin );

	return transformedValue + prevValue;
}

template <class T>
T GEControllerInputKey<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}

template <class T>
void GEControllerInputKey<T>::ProcessInput( const GEInputState* inputState )
{
	// Set the previous pressed state.
	this->pressedPrev = this->pressed;
	
	// Update the key status
	this->pressed = inputState->getKeyboardKey( this->key );
	
}

typedef GEControllerInputKey<float> GEControllerInputKeyf1;
typedef GEControllerInputKey<GEvec2> GEControllerInputKeyv2;
typedef GEControllerInputKey<GEvec3> GEControllerInputKeyv3;

#endif /* GECONTROLLERINPUTKEY_H */