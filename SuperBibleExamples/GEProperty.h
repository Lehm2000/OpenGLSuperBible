#ifndef GEPROPERTY_H
#define GEPROPERTY_H

/**
	GEProperty
	Purpose:  To hold a value that can be modified through a list of controllers.
	Author: Jeff Adams
*/



template <class T>
class GEProperty
{
private:
	T value;
	std::vector<GEController*> controllers;

public:

	// Structors
	GEProperty()
	{
	}

	GEProperty( T value )
	{
		this->value = value;
	}

	GEProperty( &GEProperty<T> source )
	{
		this->value = source.value;
	}
};

#endif /* GEPROPERTY_H */