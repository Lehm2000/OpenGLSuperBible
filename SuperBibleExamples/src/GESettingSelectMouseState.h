#ifndef GESETTINGSELECTMOUSESTATE_H
#define GESETTINGSELECTMOUSESTATE_H

/**
	GESettingSelectMouseState
	GESetting class that selects the value to use based on the mouse state of the parent object.
*/

#include <vector>
#include "InputItem.h"

#include "GESetting.h"

#define GE_MOUSE_NOTOVER	0
#define GE_MOUSE_OVER		1
#define GE_MOUSE_DOWN		2
#define GE_MOUSE_UP			3

template <class T>
class GESettingSelectMouseState: public GESetting<T>
{
private:

	// Members

	int mouseStateValueIndex[4];  // Keeps track of what the setting value index should be for each of the 4 mouse states( MOUSE_NOT_OVER, MOUSE_OVER, MOUSE_DOWN, MOUSE_UP )


public:

	// Structors

	GESettingSelectMouseState();
	GESettingSelectMouseState( const unsigned char valueIndex, const std::vector<T> valueList );
	GESettingSelectMouseState( const GESettingSelectMouseState& source );
	~GESettingSelectMouseState();

	// Setters and Getters

	void setMouseStateValueIndex( const int index, const int value );

	int getMouseStateValueIndex( const int index ) const;
	
	// Functions

	virtual void Update( const GEObject* parent, const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime );

	virtual void ProcessInput( const GEInputState* inputState );

	virtual GESetting<T>* clone() const;
		
};

// Structors

template <class T>
GESettingSelectMouseState<T>::GESettingSelectMouseState()
	:GESetting()
{
	
	this->setMouseStateValueIndex( 0, 0 );
	this->setMouseStateValueIndex( 1, 0 );
	this->setMouseStateValueIndex( 2, 0 );
	this->setMouseStateValueIndex( 3, 0 );
}

template <class T>
GESettingSelectMouseState<T>::GESettingSelectMouseState( const unsigned char valueIndex, const std::vector<T> valueList )
	:GESetting( valueIndex, valueList )
{
	this->setMouseStateValueIndex( 0, 0 );
	this->setMouseStateValueIndex( 1, 0 );
	this->setMouseStateValueIndex( 2, 0 );
	this->setMouseStateValueIndex( 3, 0 );
}


template <class T>
GESettingSelectMouseState<T>::GESettingSelectMouseState( const GESettingSelectMouseState& source )
	:GESetting( source.valueIndex, source.valueList )
{
	
	this->setMouseStateValueIndex( 0, source.getMouseStateValueIndex( 0 ) );
	this->setMouseStateValueIndex( 1, source.getMouseStateValueIndex( 1 ) );
	this->setMouseStateValueIndex( 2, source.getMouseStateValueIndex( 2 ) );
	this->setMouseStateValueIndex( 3, source.getMouseStateValueIndex( 3 ) );
}

template <class T>
GESettingSelectMouseState<T>::~GESettingSelectMouseState()
{
}


// Setters and Getters
template <class T>
void GESettingSelectMouseState<T>::setMouseStateValueIndex( const int index, const int value )
{
	this->mouseStateValueIndex[ index ] = value;
}

template <class T>
int GESettingSelectMouseState<T>::getMouseStateValueIndex( const int index ) const
{
	return this->mouseStateValueIndex[ index ];
}



// Functions

template <class T>
void GESettingSelectMouseState<T>::Update( const GEObject* parent, const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime )
{
	// Update the material based on the parent objects mouse over setting
	if( parent != nullptr )
	{
		if( parent->isMouseOver() )
		{
			valueIndex = getMouseStateValueIndex( GE_MOUSE_OVER );
		}
		else
		{
			valueIndex = getMouseStateValueIndex( GE_MOUSE_NOTOVER );
		}
	}
}

template <class T>
void GESettingSelectMouseState<T>::ProcessInput( const GEInputState* inputState )
{
	if( inputState != nullptr )
	{
		// do nothing for now.
	}
}

template <class T>
GESetting<T>* GESettingSelectMouseState<T>::clone() const
{
	return new GESettingSelectMouseState<T>(*this);
}

#endif /* GESETTINGSELECTMOUSESTATE_H */