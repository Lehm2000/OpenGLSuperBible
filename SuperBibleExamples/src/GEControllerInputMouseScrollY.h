#ifndef GECONTROLLERINPUTMOUSESCROLLY_H
#define GECONTROLLERINPUTMOUSESCROLLY_H

/**
	GEControllerInputMousePosition
	Purpose: GEController that monitors the X position of the mouse and applies the deltaVec as a multiple of the offset.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"
#include "GEInputState.h"
#include "TypeDefinitions.h"



template <class T>
class GEControllerInputMouseScrollY: public GEControllerConstant<T>
{
private:
	float mouseScrollYDelta;

public:
	GEControllerInputMouseScrollY();
	GEControllerInputMouseScrollY( const T valueDelta );
	GEControllerInputMouseScrollY( const GEControllerInputMouseScrollY<T>& source );
	virtual ~GEControllerInputMouseScrollY();

	// Setters
	//virtual void setGameEntities( const GEObjectContainer* gameEntities );  // override from GEController

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerInputMouseScrollY<T>* clone() const;

	/**
		Update()
		Takes objectVector and applies the deltaVec to it if this objects key is pressed.
		@param startVector - starting point of the object.  Could be position, rotation or scale
		@param gameTime - time since the game started
		@param deltaTime - time since the last frame
		@return
	*/
	virtual T Control( const GEObject* parent, const GEObjectContainer* gameEntities, const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin );

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

// Structors

template <class T>
GEControllerInputMouseScrollY<T>::GEControllerInputMouseScrollY()
	:GEControllerConstant<T>()
{
	this->mouseScrollYDelta = 0.0f;
}

template <class T>
GEControllerInputMouseScrollY<T>::GEControllerInputMouseScrollY( const T valueDelta )
	:GEControllerConstant<T>( valueDelta )
{
	this->mouseScrollYDelta = 0.0f;
}

template <class T>
GEControllerInputMouseScrollY<T>::GEControllerInputMouseScrollY( const GEControllerInputMouseScrollY<T>& source )
	:GEControllerConstant<T>( source.valueDelta )
{
	this->mouseScrollYDelta = 0.0f;
}

// Setters
/*
template <class T>
void GEControllerInputMouseScrollY<T>::setGameEntities( const GEObjectContainer* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;


}
*/
// Functions

template <class T>
GEControllerInputMouseScrollY<T>::~GEControllerInputMouseScrollY()
{
}


template <class T>
GEControllerInputMouseScrollY<T>* GEControllerInputMouseScrollY<T>::clone() const
{
	return new GEControllerInputMouseScrollY<T>( *this );
}


template <class T>
T GEControllerInputMouseScrollY<T>::Control( const GEObject* parent, const GEObjectContainer* gameEntities, const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin )
{
	
	// apply the change. x = y rotation, y = x rotation
	transformedValue += valueDelta * mouseScrollYDelta;

	transformedValue = ValidateRange( transformedValue, prevValue, max, useMax, min, useMin );

	mouseScrollYDelta = 0.0f;

	return transformedValue + prevValue;
}


template <class T>
T GEControllerInputMouseScrollY<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}

template <class T>
void GEControllerInputMouseScrollY<T>::ProcessInput( const GEInputState* inputState )
{
	this->mouseScrollYDelta = inputState->getMouseScrollOffset().y;
}


typedef GEControllerInputMouseScrollY<float> GEControllerInputMouseScrollYf1;
typedef GEControllerInputMouseScrollY<GEvec2> GEControllerInputMouseScrollYv2;
typedef GEControllerInputMouseScrollY<GEvec3> GEControllerInputMouseScrollYv3;


#endif /* GECONTROLLERINPUTMOUSESCROLLX_H */