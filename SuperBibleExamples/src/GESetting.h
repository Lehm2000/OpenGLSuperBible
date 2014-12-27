#ifndef GESETTING_H
#define GESETTING_H

/**
	GESetting
	Class to hold a controllable setting.  Similar to GEProperty but intended to hold a list of specified values.  Since
	each setting is intended to be controlled in a single way, a stack of controllers is not needed.  Instead different
	behaviours will be added by creating child classes.
*/

#include <vector>
#include "InputItem.h"

//#include "GEObjectContainer.h"

template <class T>
class GESetting
{
protected:

	// Members

	unsigned char valueIndex;				// index of current value in valueList
	std::vector<T> valueList;				// list of possible values that will be cycled through ( the default setting should be index 0 );
	

	//const GEObjectContainer* gameEntities;	// pointer to the game entities, mostly so it can get access to the inputState.  TODO: is it better to give it a pointer to the inputstate?  What happens in the event the inputstate gets destroyed and recreated?

public:

	// Structors
	GESetting();
	GESetting( const unsigned char valueIndex, const std::vector<T> valueList );
	GESetting( const GESetting& source );
	virtual ~GESetting();

	// Setters

	/**
		setValueIndex()
		Takes value and matches it to something in valueList.  Sets valueIndex to found index
	*/
	void setValueIndex( const unsigned char valueIndex );

	/**
		setValue()
		Takes value and matches it to something in valueList.  Sets valueIndex to found index
	*/
	void setValue( const T value );

	/**
		setValueList()
	*/
	void setValueList( const std::vector<T> valueList );

	

	// Getters

	T getValue() const;
	const std::vector<T> getValueList() const;
	

	// Functions
	virtual void Update( const GEObject* parent, const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime ) = 0;

	virtual void ProcessInput( const GEInputState* inputState ) = 0;

	virtual GESetting<T>* clone() const = 0;

	
		
};

// Structors

template <class T>
GESetting<T>::GESetting()
{
	this->valueIndex = 0;
	
}

template <class T>
GESetting<T>::GESetting( const unsigned char valueIndex, const std::vector<T> valueList)
{
	this->setValueIndex( valueIndex );
	this->setValueList( valueList );
	
}


template <class T>
GESetting<T>::GESetting( const GESetting& source )
{
	this->setValueIndex( source.valueIndex );
	this->setValueList( source.valueList );

}

template <class T>
GESetting<T>::~GESetting()
{
}


// Setters

template <class T>
void GESetting<T>::setValueIndex( const unsigned char valueIndex )
{
	if (valueIndex >= 0 && valueIndex < valueList.size() )
	{
		this->valueIndex = valueIndex;
	}
}

template <class T>
void GESetting<T>::setValue( const T value )
{
	// go through the list of values and see if one matches

	for( unsigned int i = 0; i < valueList.size(); i++ )
	{
		if( valueList[i] == value )
		{
			// found one.
			this->setValueIndex( i );
			return;
		}
	}
}


template <class T>
void GESetting<T>::setValueList( const std::vector<T> valueList )
{
	this->valueList = valueList;
}


// Getters

template <class T>
T GESetting<T>::getValue() const
{
	return this->valueList[ this->valueIndex ];
}

template <class T>
const std::vector<T> GESetting<T>::getValueList() const
{
	return this->valueList;
}

// Functions




#endif /* GESETTING_H */