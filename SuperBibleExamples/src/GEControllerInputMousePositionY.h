#ifndef GECONTROLLERINPUTMOUSEPOSITIONY_H
#define GECONTROLLERINPUTMOUSEPOSITIONY_H

/**
	GEControllerInputMousePosition
	Purpose: GEController that monitors the X position of the mouse and applies the deltaVec as a multiple of the offset.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"

template <class T>
class GEControllerInputMousePositionY: public GEControllerConstant<T>
{
private:
	
	float mousePositionY;			// where is the mouse
	float mousePositionYPrev;		// where was the mouse

public:
	GEControllerInputMousePositionY();
	GEControllerInputMousePositionY( const T valueDelta );
	GEControllerInputMousePositionY( const GEControllerInputMousePositionY<T>& source );
	virtual ~GEControllerInputMousePositionY();

	// Setters
	//virtual void setGameEntities( const GEObjectContainer* gameEntities );  // override from GEController

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerInputMousePositionY<T>* clone() const;

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
GEControllerInputMousePositionY<T>::GEControllerInputMousePositionY()
	:GEControllerConstant<T>()
{
	this->mousePositionY = 0.0f;
	this->mousePositionYPrev = 0.0f;
}

template <class T>
GEControllerInputMousePositionY<T>::GEControllerInputMousePositionY( T valueDelta )
	:GEControllerConstant<T>( valueDelta )
{
	this->mousePositionY = 0.0f;
	this->mousePositionYPrev = 0.0f;
	
}

template <class T>
GEControllerInputMousePositionY<T>::GEControllerInputMousePositionY( const GEControllerInputMousePositionY<T>& source )
	:GEControllerConstant<T>( source.valueDelta )
{
	this->mousePositionY = source.mousePositionY;
	this->mousePositionYPrev = source.mousePositionYPrev;
}

// Setters
/*
template <class T>
void GEControllerInputMousePositionY<T>::setGameEntities( const GEObjectContainer* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;

	// once the gameEntities is set... now we can get the mouse position from the input state object.
	//std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );

	const GEObject* isObject = gameEntities->GetObject( "SYS_Input_State" );
	
	if ( isObject != nullptr )
	{
		const InputStateHolder* inputStateHolder = (InputStateHolder*)isObject;
		const GEInputState* inputState = inputStateHolder->getInputState();
		this->mousePositionY = inputState->getMousePosition().y;
		this->mousePositionYPrev = this->mousePositionY;
	}
}
*/
// Functions

template <class T>
GEControllerInputMousePositionY<T>::~GEControllerInputMousePositionY()
{
}


template <class T>
GEControllerInputMousePositionY<T>* GEControllerInputMousePositionY<T>::clone() const
{
	return new GEControllerInputMousePositionY( *this );
}


template <class T>
T GEControllerInputMousePositionY<T>::Control( const GEObject* parent, const GEObjectContainer* gameEntities, const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin )
{
	
	// find the change
	float mousePosXDelta = this->mousePositionY - this->mousePositionYPrev;

	// apply the change. x = y rotation, y = x rotation
	transformedValue += valueDelta * mousePosXDelta;

	transformedValue = ValidateRange( transformedValue, prevValue, max, useMax, min, useMin );

	return transformedValue + prevValue;
}


template <class T>
T GEControllerInputMousePositionY<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}

template <class T>
void GEControllerInputMousePositionY<T>::ProcessInput( const GEInputState* inputState )
{
	if( inputState->getMouseMode() == GE_MOUSEMODE_LOOK )
	{
		this->mousePositionYPrev = inputState->getMousePositionPrev().y;

		this->mousePositionY = inputState->getMousePosition().y;
	}
	else
	{
		this->mousePositionYPrev = this->mousePositionY = inputState->getMousePosition().y;
	}
	

}


typedef GEControllerInputMousePositionY<float> GEControllerInputMousePositionYf1;
typedef GEControllerInputMousePositionY<GEvec2> GEControllerInputMousePositionYv2;
typedef GEControllerInputMousePositionY<GEvec3> GEControllerInputMousePositionYv3;


#endif /* GECONTROLLERINPUTMOUSEPOSITIONY_H */