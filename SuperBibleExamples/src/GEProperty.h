#ifndef GEPROPERTY_H
#define GEPROPERTY_H

/**
	GEProperty
	Purpose:  To hold a value that can be modified through a list of controllers.
	Author: Jeff Adams
*/

#include "GEObject.h"
#include "GEController.h"

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
	T getControlledValue() const;

	// Operators
	GEProperty& operator=( const GEProperty& source );

	// Functions
	void addPositionController( GEController<T>* controller );
	void removePositionController( const unsigned int index );
	void setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities);
	
};

// Structors

template <class T>
GEProperty<T>::GEProperty()
{
}

template <class T>
GEProperty<T>::GEProperty( T value )
{
}

template <class T>
GEProperty<T>::GEProperty( GEProperty& source )
{
}

template <class T>
GEProperty<T>::~GEProperty();

// Setters

template <class T>
void GEProperty<T>::setValue( const T value )
{
}


// Getters


template <class T>
T GEProperty<T>::getBaseValue() const
{
}

template <class T>
T GEProperty<T>::getControlledValue() const
{
}


// Operators

template <class T>
GEProperty& GEProperty<T>::operator=( const GEProperty& source )
{
}


// Functions

template <class T>
void GEProperty<T>::addPositionController( GEController<T>* controller )
{
}

template <class T>
void GEProperty<T>::removePositionController( const unsigned int index )
{
}

template <class T>
void GEProperty<T>::setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities )
{
}


#endif /* GEPROPERTY_H */