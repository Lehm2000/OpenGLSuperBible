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
	

public:
	GEControllerInputMouseScrollY();
	GEControllerInputMouseScrollY( const T valueDelta );
	GEControllerInputMouseScrollY( const GEControllerInputMouseScrollY<T>& source );
	virtual ~GEControllerInputMouseScrollY();

	// Setters
	virtual void setGameEntities( const GEObjectContainer* gameEntities );  // override from GEController

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
	virtual T Control( const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin );

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual T CalcTransform( const T sourceValue );

	
};

// Structors

template <class T>
GEControllerInputMouseScrollY<T>::GEControllerInputMouseScrollY()
	:GEControllerConstant<T>()
{

}

template <class T>
GEControllerInputMouseScrollY<T>::GEControllerInputMouseScrollY( const T valueDelta )
	:GEControllerConstant<T>( valueDelta )
{
	
}

template <class T>
GEControllerInputMouseScrollY<T>::GEControllerInputMouseScrollY( const GEControllerInputMouseScrollY<T>& source )
	:GEControllerConstant<T>( source.valueDelta )
{
}

// Setters

template <class T>
void GEControllerInputMouseScrollY<T>::setGameEntities( const GEObjectContainer* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;


}

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
T GEControllerInputMouseScrollY<T>::Control( const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin )
{
	float mouseScrollYDelta = 0.0f;
	

	// get the new mouse position
	//std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );

	const GEObject* isObject = gameEntities->GetObject( "SYS_Input_State" );
	
	if ( isObject != nullptr )
	{
		const GEInputState* inputState = (GEInputState*)isObject;
		mouseScrollYDelta = inputState->getMouseScrollOffset().y;
	}

	// apply the change. x = y rotation, y = x rotation
	transformedValue += valueDelta * mouseScrollYDelta;

	transformedValue = ValidateRange( transformedValue, prevValue, max, useMax, min, useMin );

	return transformedValue + prevValue;
}


template <class T>
T GEControllerInputMouseScrollY<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}


typedef GEControllerInputMouseScrollY<float> GEControllerInputMouseScrollYf1;
typedef GEControllerInputMouseScrollY<GEvec2> GEControllerInputMouseScrollYv2;
typedef GEControllerInputMouseScrollY<GEvec3> GEControllerInputMouseScrollYv3;


#endif /* GECONTROLLERINPUTMOUSESCROLLX_H */