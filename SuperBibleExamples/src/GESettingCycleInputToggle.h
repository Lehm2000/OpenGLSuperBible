#ifndef GESETTINGCYCLEINPUTTOGGLE_H
#define GESETTINGCYCLEINPUTTOGGLE_H

/**
	GESettingCycleInputToggle
	GESetting class that cycles through the list of specified values
	when the specified GE_ENGINE_ACTION is toggled.  Keeps track of how it was toggled to generate its own GE_ACTION
*/

#include <vector>
#include "InputItem.h"

#include "GESetting.h"

template <class T>
class GESettingCycleInputToggle: public GESetting<T>
{
private:

	// Members

	unsigned int engineAction;				// What action toggle are we listening for.  Member of GE_ENGINE_ACTION_*
	unsigned char action;					// What type of toggle action are we looking for.  Member of GE_ACTION_*
	bool toggledPrev;						// Was it toggle on the previous frame.  Used to match actionType.

	//const GEObjectContainer* gameEntities;	// pointer to the game entities, mostly so it can get access to the inputState.  TODO: is it better to give it a pointer to the inputstate?  What happens in the event the inputstate gets destroyed and recreated?

public:

	// Structors

	GESettingCycleInputToggle();
	GESettingCycleInputToggle( const unsigned char valueIndex, const std::vector<T> valueList, unsigned int engineAction, unsigned char action );
	GESettingCycleInputToggle( const GESettingCycleInputToggle& source );
	~GESettingCycleInputToggle();

	// Setters and Getters


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

	
	unsigned int getEngineAction() const;
	unsigned char getAction() const;

	// Functions
	virtual void ProcessInput( const GEInputState* inputState );

	virtual GESetting<T>* clone() const;

	/**
		CycleSetting()
		Sets the value to the next item in the list.
	*/
	void CycleSetting();
		
};

// Structors

template <class T>
GESettingCycleInputToggle<T>::GESettingCycleInputToggle()
	:GESetting()
{
	
	this->toggledPrev = false;
}

template <class T>
GESettingCycleInputToggle<T>::GESettingCycleInputToggle( const unsigned char valueIndex, const std::vector<T> valueList, unsigned int engineAction, unsigned char action )
	:GESetting( valueIndex, valueList )
{
	
	this->setEngineAction( engineAction );
	this->setAction( action );
	this->toggledPrev = false;
}


template <class T>
GESettingCycleInputToggle<T>::GESettingCycleInputToggle( const GESettingCycleInputToggle& source )
	:GESetting( source.valueIndex, source.valueList )
{
	
	this->setEngineAction( source.engineAction );
	this->setAction( source.action );
	this->toggledPrev = source.toggledPrev;
}

template <class T>
GESettingCycleInputToggle<T>::~GESettingCycleInputToggle()
{
}


// Setters and Getters

template <class T>
void GESettingCycleInputToggle<T>::setEngineAction( const unsigned int engineAction )
{
	this->engineAction = engineAction;
}


template <class T>
unsigned int GESettingCycleInputToggle<T>::getEngineAction() const
{
	return this->engineAction;
}

template <class T>
void GESettingCycleInputToggle<T>::setAction( const unsigned char action )
{
	this->action = action;
}

template <class T>
unsigned char GESettingCycleInputToggle<T>::getAction() const
{
	return this->action;
}


// Functions
template <class T>
void GESettingCycleInputToggle<T>::ProcessInput( const GEInputState* inputState )
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
GESetting<T>* GESettingCycleInputToggle<T>::clone() const
{
	return new GESettingCycleInputToggle(*this);
}

template <class T>
void GESettingCycleInputToggle<T>::CycleSetting()
{
	this->valueIndex = (valueIndex + 1) % valueList.size();
}

#endif /* GESETTINGCYCLEINPUTTOGGLE_H */