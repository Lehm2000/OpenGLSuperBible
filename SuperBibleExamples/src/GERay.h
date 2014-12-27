#ifndef GERAY_H
#define GERAY_H

/**
	GERay
	Class to define a ray object.

	Author: Jeff Adams
	2014-Nov-15
*/


#include "TypeDefinitions.h"

class GERay
{
private:

	// Members

	GEvec3 origin;
	GEvec3 direction;

public:

	// Structors

	GERay();
	GERay( GEvec3 origin, GEvec3 direction );
	// copy if needed
	// destructor if needed

	// Getters and Setters
	void setOrigin( const GEvec3 origin );
	GEvec3 getOrigin() const;

	void setDirection( const GEvec3 direction );
	GEvec3 getDirection() const;

	// Operators

	GERay operator*( glm::mat4& matrix );

};

#endif