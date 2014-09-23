#ifndef GECONTROLLERINPUTMOUSEPOSITIONX_H
#define GECONTROLLERINPUTMOUSEPOSITIONX_H

/**
	GEControllerInputMousePosition
	Purpose: GEController that monitors the X position of the mouse and applies the deltaVec as a multiple of the offset.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"



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
	virtual void setGameEntities( const std::map< std::string, GEObject* >* gameEntities );  // override from GEController

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
	virtual void Control( T initialValue, double gameTime, double deltaTime);

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual T CalcTransform( T sourceValue );

	
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

template <class T>
void GEControllerInputMousePositionX<T>::setGameEntities( const std::map< std::string, GEObject* >* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;

	// once the gameEntities is set... now we can get the mouse position from the input state object.
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->mousePositionX = inputState->getMousePosition().x;
		this->mousePositionXPrev = this->mousePositionX;
	}
}

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
void GEControllerInputMousePositionX<T>::Control( T initialValue, double gameTime, double deltaTime)
{
	// Start by updating the mouse position
	this->mousePositionXPrev = this->mousePositionX;

	// get the new mouse position
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->mousePositionX = inputState->getMousePosition().x;
	}

	// find the change
	float mousePosXDelta = this->mousePositionX - this->mousePositionXPrev;

	// apply the change. x = y rotation, y = x rotation
	transformedValue += valueDelta * mousePosXDelta;
}


template <class T>
T GEControllerInputMousePositionX<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}

typedef GEControllerInputMousePositionX<float> GEControllerInputMousePositionXf1;
typedef GEControllerInputMousePositionX<glm::vec2> GEControllerInputMousePositionXv2;
typedef GEControllerInputMousePositionX<glm::vec3> GEControllerInputMousePositionXv3;


#endif /* GECONTROLLERINPUTMOUSEPOSITIONX_H */