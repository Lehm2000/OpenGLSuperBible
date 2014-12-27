#ifndef GEPLANE_H
#define GEPLANE_H

/**
	GEPlane

	Author Jeff Adams
	2014-Nov-15
*/

#include "TypeDefinitions.h"
#include "GERay.h"

class GEPlane
{
private:
	// members

	GEvec3 normal;		// direction the plane is facing
	GEvec3 point1;		// a point on the plane
	GEvec3 point2;		// a second point on the plane.  Not really needed, could be calculated with normal and point1... but its faster to have it around.
	
public:
	// structors

	GEPlane();
	GEPlane( GEvec3 point1, GEvec3 point2, GEvec3 point3 );

	// Getters ... no setters for this class the members need to be calculated.

	GEvec3 getNormal() const;
	GEvec3 getPoint1() const;
	GEvec3 getPoint2() const;

	// Functions

	/**
		Use three points to calculate new plane.
	*/
	void setPlane( GEvec3 A, GEvec3 B, GEvec3 C );

	/**
		Return the point where point intersects this plane... or nullptr if no intersection
	*/
	GEvec3* intersectRay( GERay ray );
};


#endif /* GEPLANE_H */