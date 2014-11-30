#ifndef GESETTING_H
#define GESETTING_H

/**
	GESetting
	Class to hold a controllable setting.  Similar to GEProperty but intended to cycle through a list of specified values
	when the specified GE_ENGINE_ACTION is toggled.  Keeps track of how it was toggled to generate its own GE_ACTION
*/

#include <vector>
#include "InputItem.h"

//#include "GEObjectContainer.h"

template <class T>
class GESetting
{
private:

	// Members

	unsigned char valueIndex;				// index of current value in valueList
	std::vector<T> valueList;				// list of possible values that will be cycled through ( the default setting should be index 0 );
	unsigned int engineAction;				// What action toggle are we listening for.  Member of GE_ENGINE_ACTION_*
	unsigned char action;					// What type of toggle action are we looking for.  Member of GE_ACTION_*
	bool toggledPrev;						// Was it toggle on the previous frame.  Used to match actionType.

	//const GEObjectContainer* gameEntities;	// pointer to the game entities, mostly so it can get access to the inputState.  TODO: is it better to give it a pointer to the inputstate?  What happens in the event the inputstate gets destroyed and recreated?

public:

	// Structors
	GESetting();
	GESetting( const unsigned char valueIndex, const std::vector<T> valueList, unsigned int engineAction, unsigned char action );
	GESetting( const GESetting& source );
	~GESetting();

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

	/**
		setEngineAction()
		Sets the GE_ENGINE_ACTION_* that will cycle this setting
	*/
	void setEngineAction( const unsigned int engineAction );

	/**
		setAction()
		Sets the GE_ACTION_* we are looking for
	*/
	void setAction( const unsigned char action );

	// Getters

	T getValue() const;
	unsigned int getEngineAction() const;
	unsigned char getAction() const;

	// Functions
	void ProcessInput( const GEInputState* inputState );

	/**
		CycleSetting()
		Sets the value to the next item in the list.
	*/
	void CycleSetting();
		
};

// Structors

template <class T>
GESetting<T>::GESetting()
{
	this->valueIndex = 0;
	this->toggledPrev = false;
}

template <class T>
GESetting<T>::GESetting( const unsigned char valueIndex, const std::vector<T> valueList, unsigned int engineAction, unsigned char action )
{
	this->setValueIndex( valueIndex );
	this->setValueList( valueList );
	this->setEngineAction( engineAction );
	this->setAction( action );
	this->toggledPrev = false;
}


template <class T>
GESetting<T>::GESetting( const GESetting& source )
{
	this->setValueIndex( source.valueIndex );
	this->setValueList( source.valueList );
	this->setEngineAction( source.engineAction );
	this->setAction( source.action );
	this->toggledPrev = source.toggledPrev;
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

template <class T>
void GESetting<T>::setEngineAction( const unsigned int engineAction )
{
	this->engineAction = engineAction;
}

template <class T>
void GESetting<T>::setAction( const unsigned char action )
{
	this->action = action;
}


// Getters

template <class T>
T GESetting<T>::getValue() const
{
	return this->valueList[ this->valueIndex ];
}

template <class T>
unsigned int GESetting<T>::getEngineAction() const
{
	return this->engineAction;
}

template <class T>
unsigned char GESetting<T>::getAction() const
{
	return this->action;
}

// Functions
template <class T>
void GESetting<T>::ProcessInput( const GEInputState* inputState )
{
	if( inputState != nullptr )
	{
		// get if the Engine Action we are looking for is currently toggled (all keys/buttons pressed)
		bool toggled = inputState->ActionToggled( this->engineAction );

		// compare to the type of toggle action we are looking for.
		
		if ( this->action == GE_ACTION_PRESS )
		{
			if( toggled && !toggledPrev )  // if combination pressed.
			{
				this->CycleSetting();
			}
		}
		else if ( this->action == GE_ACTION_RELEASE )
		{
			if( !toggled && toggledPrev )  // if combination released
			{
				this->CycleSetting();
			}
		}
		else if ( this->action == GE_ACTION_REPEAT )
		{
			if( toggled ) // if combination down... doesn't matter what the previous frame was
			{
				this->CycleSetting();
			}
		}

		toggledPrev = toggled;
	}
}

template <class T>
void GESetting<T>::CycleSetting()
{
	this->valueIndex = (valueIndex + 1) % valueList.size();
}

#endif /* GESETTING_H */