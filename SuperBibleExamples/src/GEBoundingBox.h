#ifndef GEBOUNDINGBOX_H
#define GEBOUNDINGBOX_H

/**
	GEBoundingBox

	Author Jeff Adams
	2014-Nov-14
*/

#include "TypeDefinitions.h"
#include "GERay.h"

class GEBoundingBox
{
private:

	// Members

	GEvec3 min;		// the minimum xyz values
	GEvec3 max;		// the maximum xyz values

public:

	// Structors

	GEBoundingBox();
	GEBoundingBox( GEvec3 min, GEvec3 max );
	// copy constructor if needed
	// destructor if needed

	// setters and getters
	void setMin( const GEvec3 min );
	GEvec3 getMin() const;

	void setMax( const GEvec3 max );
	GEvec3 getMax() const;

	// Functions

	/**
		Calculates the point where a ray intersects the box.  Returns null if no intersection.  Ray should already be 
		transformed to the object space of the box
	*/
	GEvec3* intersectRay( const GERay ray );
};

#endif /* GEBOUNDINGBOX_H */