#ifndef GECONTROLLEROSCILLATOR_H
#define GECONTROLLEROSCILLATOR_H

/**
	GEControllerOscillator
	Purpose: GEController object that oscillates at a given amplitude an frequency.
	Author: Jeff Adams
*/

#include <glm\glm.hpp>

#include "GEController.h"
#include "GEConstants.h"

template <class T>
class GEControllerOscillator: public GEController<T>
{
private:
	// Members

	T amplitude;  // max change per second 
	float frequency;	// how often (in seconds)

public:
	//Structors

	GEControllerOscillator();
	GEControllerOscillator( T amplitude, float frequency );
	GEControllerOscillator( const GEControllerOscillator<T>& source );
	virtual ~GEControllerOscillator();

	// Setters
	void setAmplitude( T amplitude );
	void setFrequency( const float frequency );

	// Getters
	T getAmplitude() const;
	float getFrequency() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerOscillator<T>* clone() const;

	/**
		Update()
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

//Structors

template <class T>
GEControllerOscillator<T>::GEControllerOscillator()
{
}

template <class T>
GEControllerOscillator<T>::GEControllerOscillator( T amplitude, float frequency )
	:GEController<T>()
{
	this->setAmplitude( amplitude  );
	this->setFrequency( frequency );
}

template <class T>
GEControllerOscillator<T>::GEControllerOscillator( const GEControllerOscillator<T>& source )
	:GEController<T>( source.parent, source.gameEntities )
{
	this->setAmplitude( source.amplitude );
	this->frequency = source.frequency;	// can't use setter for this as it modifies incoming value.
}

template <class T>
GEControllerOscillator<T>::~GEControllerOscillator()
{
}


// Setters

template <class T>
void GEControllerOscillator<T>::setAmplitude( T amplitude )
{
	this->amplitude = amplitude;
}

template <class T>
void GEControllerOscillator<T>::setFrequency( const float frequency )
{
	
	this->frequency = frequency;
}


// Getters

template <class T>
T GEControllerOscillator<T>::getAmplitude() const
{
	return this-> amplitude;
}

template <class T>
float GEControllerOscillator<T>::getFrequency() const
{
	return this->frequency;
}


// Functions

template <class T>
GEControllerOscillator<T>* GEControllerOscillator<T>::clone() const
{
	return new GEControllerOscillator<T>( *this );
}

template <class T>
void GEControllerOscillator<T>::Control( T initialValue, double gameTime, double deltaTime)
{
	float newFeq = frequency / ( 2.0f * GE_PI ); // convert the frequence to terms of PI
	transformedValue = sin( (float)gameTime / newFeq ) * amplitude;
}


template <class T>
T GEControllerOscillator<T>::CalcTransform( T sourceValue )
{
	return sourceValue + transformedValue;
}

typedef GEControllerOscillator<float> GEControllerOscillatorf1;
typedef GEControllerOscillator<GEvec2> GEControllerOscillatorv2;
typedef GEControllerOscillator<GEvec3> GEControllerOscillatorv3;

#endif /* GECONTROLLEROSCILLATOR_H */