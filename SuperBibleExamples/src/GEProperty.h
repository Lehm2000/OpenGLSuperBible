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
	GEProperty()
	GEProperty( T value )
	GEProperty( GEProperty& source )
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

#endif /* GEPROPERTY_H */