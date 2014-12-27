#ifndef GECONTROLLERINPUTMOUSEPOSITIONX_H
#define GECONTROLLERINPUTMOUSEPOSITIONX_H

/**
	GEControllerInputMousePosition
	Purpose: GEController that monitors the X position of the mouse and applies the deltaVec as a multiple of the offset.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"
#include "InputStateHolder.h"
#include "TypeDefinitions.h"



template <class T>
class GEControllerInputMousePositionX: public GEControllerConstant<T>
{
private:
	
	float mousePositionX;			// where is the mouse
	float mousePositionXPrev;		// where was the mouse

public:
	GEControllerInputMousePositionX();
	GEControllerInputMousePositionX( const T valueDelta );
	GEControllerInputMousePositionX( const GEControllerInputMousePositionX<T>& source );
	virtual ~GEControllerInputMousePositionX();

	// Setters
	// virtual void setGameEntities( const GEObjectContainer* gameEntities );  // override from GEController

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerInputMousePositionX<T>* clone() const;

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
GEControllerInputMousePositionX<T>::GEControllerInputMousePositionX()
	:GEControllerConstant<T>()
{
	this->mousePositionX = 0.0f;
	this->mousePositionXPrev = 0.0f;
}

template <class T>
GEControllerInputMousePositionX<T>::GEControllerInputMousePositionX( const T valueDelta )
	:GEControllerConstant<T>( valueDelta )
{
	this->mousePositionX = 0.0f;
	this->mousePositionXPrev = 0.0f;
	
}

template <class T>
GEControllerInputMousePositionX<T>::GEControllerInputMousePositionX( const GEControllerInputMousePositionX<T>& source )
	:GEControllerConstant<T>( source.valueDelta )
{
	this->mousePositionX = source.mousePositionX;
	this->mousePositionXPrev = source.mousePositionXPrev;
}

// Setters
/*
template <class T>
void GEControllerInputMousePositionX<T>::setGameEntities( const GEObjectContainer* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;

	// once the gameEntities is set... now we can get the mouse position from the input state object.

	const GEObject* isObject = gameEntities->GetObject( "SYS_Input_State" );
	
	if ( isObject != nullptr )
	{
		const InputStateHolder* inputStateHolder = (InputStateHolder*)isObject;
		const GEInputState* inputState = inputStateHolder->getInputState();
		this->mousePositionX = inputState->getMousePosition().x;
		this->mousePositionXPrev = this->mousePositionX;
	}
}
*/
// Functions

template <class T>
GEControllerInputMousePositionX<T>::~GEControllerInputMousePositionX()
{
}


template <class T>
GEControllerInputMousePositionX<T>* GEControllerInputMousePositionX<T>::clone() const
{
	return new GEControllerInputMousePositionX<T>( *this );
}


template <class T>
T GEControllerInputMousePositionX<T>::Control( const GEObject* parent, const GEObjectContainer* gameEntities, const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin )
{
	
	// find the change
	float mousePosXDelta = this->mousePositionX - this->mousePositionXPrev;
		
	// apply the change. x = y rotation, y = x rotation
	transformedValue += valueDelta * mousePosXDelta;

	transformedValue = ValidateRange( transformedValue, prevValue, max, useMax, min, useMin );

	return transformedValue + prevValue;
}


template <class T>
T GEControllerInputMousePositionX<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}

template <class T>
void GEControllerInputMousePositionX<T>::ProcessInput( const GEInputState* inputState )
{
	if( inputState->getMouseMode() == GE_MOUSEMODE_LOOK )
	{
		//this->mousePositionXPrev = this->mousePositionX;
		this->mousePositionXPrev = inputState->getMousePositionPrev().x;
		this->mousePositionX = inputState->getMousePosition().x;
	}
	else
	{
		this->mousePositionXPrev = this->mousePositionX = inputState->getMousePosition().x;
	}

}

typedef GEControllerInputMousePositionX<float> GEControllerInputMousePositionXf1;
typedef GEControllerInputMousePositionX<GEvec2> GEControllerInputMousePositionXv2;
typedef GEControllerInputMousePositionX<GEvec3> GEControllerInputMousePositionXv3;


#endif /* GECONTROLLERINPUTMOUSEPOSITIONX_H */