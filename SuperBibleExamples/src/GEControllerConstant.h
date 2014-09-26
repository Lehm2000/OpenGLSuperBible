#ifndef GECONTROLLERCONSTANT_H
#define GECONTROLLERCONSTANT_H

/**
	GEControllerConstant
	Purpose: GEController object that applies a constant change to the object
	Author: Jeff Adams
*/

#include <glm\glm.hpp>

#include "TypeDefinitions.h"
#include "GEController.h"


template <class T>
class GEControllerConstant: public GEController<T>
{
protected:
	// Members

	T valueDelta;  // how much to change the value per second.

public:
	//Structors

	GEControllerConstant();
	GEControllerConstant( const T valueDelta );
	GEControllerConstant( const GEControllerConstant<T>& source );
	virtual ~GEControllerConstant();

	// Setters
	void setValueDelta( const T valueDelta );

	// Getters
	T getValueDelta() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerConstant<T>* clone() const;

	/**
		Control()
		Takes objectVector and applies the deltaVec to it.
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

template <class T>
GEControllerConstant<T>::GEControllerConstant()
	:GEController()
{
}

template <class T>
GEControllerConstant<T>::GEControllerConstant( const T valueDelta )
	:GEController()
{
	this->setValueDelta( valueDelta );
}

template <class T>
GEControllerConstant<T>::GEControllerConstant( const GEControllerConstant<T>& source )
	:GEController( source.parent, source.gameEntities )
{
	this->setValueDelta( source.valueDelta );
}

template <class T>
GEControllerConstant<T>::~GEControllerConstant()
{
}

// Setters

template <class T>
void GEControllerConstant<T>::setValueDelta( const T valueDelta )
{
	this->valueDelta = valueDelta;
}

// Getters

template <class T>
T GEControllerConstant<T>::getValueDelta() const
{
	return this->valueDelta;
}

// Functions

template <class T>
GEControllerConstant<T>* GEControllerConstant<T>::clone() const
{
	return new GEControllerConstant<T>( *this );
}

template <class T>
void GEControllerConstant<T>::Control( T initialValue, double gameTime, double deltaTime)
{
	transformedValue = transformedValue + ( initialValue * (float)deltaTime  );
}

template <class T>
T GEControllerConstant<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}


typedef GEControllerConstant<float> GEControllerConstantf1;
typedef GEControllerConstant<GEvec2> GEControllerConstantv2;
typedef GEControllerConstant<GEvec3> GEControllerConstantv3;

#endif /* GECONTROLLERCONSTANT_H */