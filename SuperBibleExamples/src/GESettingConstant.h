#ifndef GESETTINGCONSTANT_H
#define GESETTINGCONSTANT_H

/**
	GESettingCycleInputToggle
	GESetting class that is constant and does not change
*/

#include <vector>
#include "InputItem.h"

#include "GESetting.h"

template <class T>
class GESettingConstant: public GESetting<T>
{
private:

	// no new members

public:

	// Structors

	GESettingConstant();
	GESettingConstant( const unsigned char valueIndex, const std::vector<T> valueList );
	GESettingConstant( const GESettingConstant& source );
	~GESettingConstant();

	// Setters and Getters


	// Functions
	virtual void ProcessInput( const GEInputState* inputState );

	virtual GESetting<T>* clone() const;

};

// Structors

template <class T>
GESettingConstant<T>::GESettingConstant()
	:GESetting()
{

}

template <class T>
GESettingConstant<T>::GESettingConstant( const unsigned char valueIndex, const std::vector<T> valueList )
	:GESetting( valueIndex, valueList )
{

}


template <class T>
GESettingConstant<T>::GESettingConstant( const GESettingConstant& source )
	:GESetting( source.valueIndex, source.valueList )
{

}

template <class T>
GESettingConstant<T>::~GESettingConstant()
{
}


// Setters and Getters



// Functions
template <class T>
void GESettingConstant<T>::ProcessInput( const GEInputState* inputState )
{
	// this setting is constant... do nothing.
}

template <class T>
GESetting<T>* GESettingConstant<T>::clone() const
{
	return new GESettingConstant<T>(*this);
}

#endif /* GESETTINGCONSTANT_H */