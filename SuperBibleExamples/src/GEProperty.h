#ifndef GEPROPERTY_H
#define GEPROPERTY_H

/**
	GEProperty
	Purpose:  To hold a value that can be modified through a list of controllers.
	Author: Jeff Adams
*/

#include <memory>  

#include "GEObject.h"
#include "GEController.h"

template <class T>
class GEController;

class GEObject;

template <class T>
class GEProperty
{
private:
	T value;										// Initial value of the property
	std::vector< GEController<T>* > controllers;	// These modify value.  They don't actually change the value of value.  Controllers either hold a change (delta) to the value or simply override it... depending on the controller type.

public:

	// Structors
	GEProperty();
	GEProperty( T value );
	GEProperty( GEProperty& source );
	~GEProperty();

	// Setters
	void setValue( const T value );

	// Getters
	T getBaseValue() const;
	T getFinalValue() const;

	// Operators
	//GEProperty& operator=( const GEProperty& source );

	// Functions
	void addController( GEController<T>* controller, GEObject* parent );
	void removeController( const unsigned int index );
	void setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities);

	void Update( const double gameTime, const double deltaTime);
	
};

// Structors

template <class T>
GEProperty<T>::GEProperty()
{
}

template <class T>
GEProperty<T>::GEProperty( T value )
{
	this->setValue( value );
}

template <class T>
GEProperty<T>::GEProperty( GEProperty& source )
{
	this->setValue( source.value );
}

template <class T>
GEProperty<T>::~GEProperty()
{
	// Delete all the controllers.
	for ( unsigned int i = 0; i < controllers.size(); i++ )
	{
		if ( controllers[i] != nullptr )
		{
			delete controllers[i];
			controllers[i] = nullptr;
		}
	}
}

// Setters

template <class T>
void GEProperty<T>::setValue( const T value )
{
	this->value = value;
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


// Operators

/*template <class T>
GEProperty<T>& GEProperty<T>::operator=( const GEProperty<T>& source )
{
	
	this->setValue( source.getInitialValue() );

	// copy the controllers from the other GEProperty
	this->controllers.clear();
	for ( unsigned int i = 0; i < source.controllers.size(); i++ )
	{
		this->addController( source.controllers[i]->clone()
			);
	}

	return *this;
}*/


// Functions

template <class T>
void GEProperty<T>::addController( GEController<T>* controller, GEObject* parent )
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
void GEProperty<T>::setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities )
{
	for ( unsigned int i = 0; i < controllers.size(); i++)
		controllers[i]->setGameEntities( gameEntities );
}

template <class T>
void GEProperty<T>::Update( const double gameTime, const double deltaTime)
{
	// go through list of controllers and tell them to do their calculations.
	for ( unsigned int i = 0; i < controllers.size(); i++)
		controllers[i]->Control( this->getBaseValue(), gameTime, deltaTime );
}

typedef GEProperty<float> GEPropertyf1;
typedef GEProperty<glm::vec2> GEPropertyv2;
typedef GEProperty<glm::vec3> GEPropertyv3;

#endif /* GEPROPERTY_H */