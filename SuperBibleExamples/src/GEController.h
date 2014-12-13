#ifndef GECONTROLLER_H
#define GECONTROLLER_H

/**
	GEController Class
	
	Purpose: 
	This is the base controller class for objects in the game.
	Controllers are the means by which objects are moved, rotated and scaled.
	The base class merely returns what was passed to it.  The object it is
	attached to basically is static then.


	Author: Jeff Adams
*/

#include <map>
#include <glm\glm.hpp>

#include "GEObject.h"
#include "GEObjectContainer.h"
#include "GEInputState.h"
#include "TypeDefinitions.h"

class GEObject;
class GEInputState;

template <class T>
class GEController
{
protected:
	// Members
	
	// const GEObject* parent;  // in case the controller needs to access the properties of the parent.  Like the lookat controller needs to know the parents position for example. 
		// TODO: parent should be const.  However we also need to get the final 
		// position of the object... the only way to do that is to return the 
		// GEproperty.  The function that returns the GEProperty can't return a
		// const version of the property so that it can be modified.  
		// Controllers need to be added to it for example.
	// const GEObjectContainer* gameEntities;  // pointer to the master gameEntity list.  In case the controller needs to know the properties of some other object in the world.  This one scares me a bit... I know its const... but is there a better way to get this info?

	T transformedValue;	// the transformed data.

public:
	// Structors
	
	GEController( );
	GEController( const GEController& source);
	virtual ~GEController();

	// Setters

	//virtual void setParent( const GEObject* parent );
	//virtual void setGameEntities( const GEObjectContainer* gameEntities );

	// Getters

	//virtual const GEObject* getParent() const;
	//virtual const GEObjectContainer* getGameEntities() const;

	// Functions 

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEController<T>* clone() const;

	/**
		Control()
		Takes objectVector and passed it on without modification.
		@param prevValue - the result of all the previous controllers.
		@param gameTime - time since the game started
		@param deltaTime - time since the last frame
		@param max - the max value
		@param useMax
		@param min
		@param useMin
		@return - the result transformedValue + prevValuer, mainly so it can be
			passed to the next controller in the stack.
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

	/**
		ValidateRange
		Takes a value adds it to prevValue and sees if it is within max and min.
		Returns a value that would stay within the confines if not.
	*/
	virtual T ValidateRange( T value, T prevValue , T max, bool useMax, T min, bool useMin );
};

// Structors
template <class T>
GEController<T>::GEController()
{
	this->transformedValue = T();

	//this->setParent( nullptr );
	//this->setGameEntities( nullptr );
}

template <class T>
GEController<T>::GEController( const GEController& source)
{
	this->transformedValue = source.transformedValue;
	//this->setParent( source.parent );	// okay to pass this pointer along
	//this->setGameEntities( source.gameEntities );	// okay to pass this pointer along
}

template <class T>
GEController<T>::~GEController()
{
	// DO NOT DELETE parent and gameEntities... I know you want to because they are pointers... trust me on this one, just don't.
}

// Setters
/*
template <class T>
void GEController<T>::setParent( const GEObject* parent )
{
	this->parent = parent;
}

template <class T>
void GEController<T>::setGameEntities( const GEObjectContainer* gameEntities )
{
	this->gameEntities = gameEntities;
}
*/

// Getters
/*
template <class T>
const GEObject* GEController<T>::getParent() const
{
	return this->parent;
}

template <class T>
const GEObjectContainer* GEController<T>::getGameEntities() const
{
	return this->gameEntities;
}
*/

// Functions

template <class T>
GEController<T>* GEController<T>::clone() const
{
	return new GEController<T>(*this);
}

template <class T>
T GEController<T>::Control( const GEObject* parent, const GEObjectContainer* gameEntities, const T prevValue, const double gameTime, const double deltaTime, T max, bool useMax, T min, bool useMin )
{
	// this version of the controller does nothing.  just pass the prevValue back
	// shouldn't be a reason to check the max/min.

	return prevValue;
	
}

template <class T>
T GEController<T>::CalcTransform( T sourceValue )
{
	return sourceValue;  //return the source as the transformed.
}

template <class T>
void GEController<T>::ProcessInput( const GEInputState* inputState )
{
	// do nothing.
	
}

template <class T>
T GEController<T>::ValidateRange( T value, T prevValue , T max, bool useMax, T min, bool useMin )
{
	T totalValue;

	// now see if the new value exceeds valid range
	if( useMax )
	{
		// see how much it can change before hitting the max
		T maxChange = max - prevValue;

		// component by component pick the lesser
		value = glm::min( maxChange, value );

	}
	if( useMin)
	{
		// see how much it can change before hitting the min
		T maxChange = min - prevValue;
		// component by component pick the greater
		value = glm::max( maxChange, value );
	}

	return value;
}

typedef GEController<float> GEControllerf1;
typedef GEController<GEvec2> GEControllerv2;
typedef GEController<GEvec3> GEControllerv3;



#endif /* GECONTROLLER_H */