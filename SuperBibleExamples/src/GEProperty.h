#ifndef GEPROPERTY_H
#define GEPROPERTY_H

/**
	GEProperty
	Purpose:  To hold a value (scalar/vector )that can be modified through a list of controllers.
	Author: Jeff Adams
*/

#include <memory>  

#include "TypeDefinitions.h"
#include "GEObject.h"
#include "GEController.h"
#include "GEObjectContainer.h"
#include "GEInputState.h"

template <class T>
class GEController;

class GEObject;

class GEInputState;

template <class T>
class GEProperty
{
private:
	T value;										// Initial value of the property
	T max;											// What is the max value of this property
	T min;											// What is the minimum value of this property
	bool useMax;									// limit the value to max
	bool useMin;									// limit the value to min
	std::vector< GEController<T>* > controllers;	// These modify value.  They don't actually change the value of value.  Controllers either hold a change (delta) to the value or simply override it... depending on the controller type.

public:

	// Structors
	GEProperty();
	GEProperty( T value, T max, bool useMax, T min, bool useMin );
	GEProperty( const GEProperty& source );
	~GEProperty();

	// Setters
	void setValue( const T value );
	void setMax( const T max );
	void setMin( const T min );
	void setUseMax( const bool useMax );
	void setUseMin( const bool useMin );

	// Getters
	T getBaseValue() const;
	T getFinalValue() const;
	T getMaxValue() const;
	T getMinValue() const;
	bool getUseMax() const;
	bool getUseMin() const;

	// Operators
	GEProperty& operator=( GEProperty<T> source );

	// Functions
	void addController( GEController<T>* controller, const GEObject* parent );
	void removeController( const unsigned int index );
	void setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities);

	void Update( const double gameTime, const double deltaTime);

	/**
		ProcessInput
		Function for processing input from the user.  Takes the input state and
		passes it to the Controllers
	*/
	void ProcessInput( const GEInputState* inputState );
	
};

// Structors

template <class T>
GEProperty<T>::GEProperty()
{
	this->setUseMax( false );
	this->setUseMin( false );
}

template <class T>
GEProperty<T>::GEProperty( T value, T max, bool useMax, T min, bool useMin )
{
	
	this->setMax( max );
	this->setUseMax( useMax );
	this->setMin( min );
	this->setUseMin( useMin );
	this->setValue( value ); // set the value last, after the validators have been set.
}

template <class T>
GEProperty<T>::GEProperty( const GEProperty& source )
{
	
	this->setMax( source.max );
	this->setUseMax( source.useMax );
	this->setMin( source.min );
	this->setUseMin( source.useMin );
	this->setValue( source.value ); // set the value last, after the validators have been set.

	// need to duplicate the controllers too.
	this->controllers.clear();
	for ( unsigned int i = 0; i< source.controllers.size(); i++ )
	{
		this->addController( source.controllers[i]->clone(), source.controllers[i]->getParent() );
	}
}

template <class T>
GEProperty<T>::~GEProperty()
{
	// Delete all the controllers.
	for ( unsigned int i = 0; i < controllers.size(); i++ )
	{
		if ( controllers[i] != nullptr )
		{
			delete controllers[i];  // controllers created with new
			controllers[i] = nullptr;
		}
	}
}

// Setters

template <class T>
void GEProperty<T>::setValue( const T value )
{
	// validate the range

	T modValue = value;

	if ( useMin )
		modValue = glm::max( modValue, getMinValue() );
	if ( useMax )
		modValue = glm::min( modValue, getMaxValue() );

	this->value = modValue;
}

template <class T>
void GEProperty<T>::setMax( const T max )
{
	this->max = max;

	// since the max changed... need to revalidate value

	if ( useMax )
		this->value = glm::min( this->value, getMaxValue() );
}

template <class T>
void GEProperty<T>::setMin( const T min )
{
	this->min = min;

	// since the min chnaged... revalidate the value

	if ( useMin )
		this->value = glm::max( value, getMinValue() );
}

template <class T>
void GEProperty<T>::setUseMax( const bool useMax )
{
	this->useMax = useMax;

	// check if existing value in range

	if ( useMax )
		this->value = glm::min( this->value, getMaxValue() );

}

template <class T>
void GEProperty<T>::setUseMin( const bool useMin )
{
	this->useMin = useMin;

	// check if existing value in range

	if ( useMin )
		this->value = glm::max( this->value, getMinValue() );
}


// Getters

template <class T>
T GEProperty<T>::getBaseValue() const
{
	return this->value;
}

template <class T>
T GEProperty<T>::getFinalValue() const
{
	// combine all the controllers values together
	T finalValue = this->getBaseValue() ;  // start with the initial value.

	for ( unsigned int i = 0; i < controllers.size(); i++)
		finalValue = controllers[i]->CalcTransform( finalValue );

	return finalValue;
}

template <class T>
T GEProperty<T>::getMaxValue() const
{
	return this->max;
}

template <class T>
T GEProperty<T>::getMinValue() const
{
	return this->min;
}

template <class T>
bool GEProperty<T>::getUseMax() const
{
	return this->useMax;
}

template <class T>
bool GEProperty<T>::getUseMin() const
{
	return this->useMin;
}


// Operators

template <class T>
GEProperty<T>& GEProperty<T>::operator=( GEProperty<T> source )
{
	std::swap( value, source.value );
	std::swap( max, source.max );
	std::swap( useMax, source.useMax );
	std::swap( min, source.min );
	std::swap( useMin, source.useMin );
	std::swap( controllers, source.controllers );
	
	return *this;
}


// Functions

template <class T>
void GEProperty<T>::addController( GEController<T>* controller, const GEObject* parent )
{
	controller->setParent( parent );
	this->controllers.push_back( controller->clone() );  // add a copy of the controller to the controller vector
}

template <class T>
void GEProperty<T>::removeController( const unsigned int index )
{
	this->controllers.erase( controllers.begin() + index );
}



template <class T>
void GEProperty<T>::setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities )
{
	for ( unsigned int i = 0; i < controllers.size(); i++)
		controllers[i]->setGameEntities( gameEntities );
}

template <class T>
void GEProperty<T>::Update( const double gameTime, const double deltaTime)
{
	T totalValue = this->getBaseValue();

	// go through list of controllers and tell them to do their calculations.
	for ( unsigned int i = 0; i < controllers.size(); i++)
		totalValue = controllers[i]->Control( totalValue, gameTime, deltaTime, max, useMax, min, useMin );
}

template <class T>
void GEProperty<T>::ProcessInput( const GEInputState* inputState )
{
	for( unsigned int i = 0; i < controllers.size(); i++ )
	{
		controllers[i]->ProcessInput( inputState );
	}
}


typedef GEProperty<float> GEPropertyf1;
typedef GEProperty<GEvec2> GEPropertyv2;
typedef GEProperty<GEvec3> GEPropertyv3;


#endif /* GEPROPERTY_H */