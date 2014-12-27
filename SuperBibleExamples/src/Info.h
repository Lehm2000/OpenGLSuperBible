#ifndef INFO_H
#define INFO_H

/**
	Info

	This is the base class of the info classes.  Info classes hold game/engine
	settings.  Extends GEObject so that they can be placed in the gameEntities

	Author: Jeff Adams
*/

#include "GEObject.h"

class Info: public GEObject
{
private:
public:
	Info();
	Info( const Info& info );

	// Functions
	
	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual Info* clone() const;

	virtual std::string getClassName() const;
};

#endif /* INFO_H */