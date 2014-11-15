// structors

#include "GEPlane.h"

GEPlane::GEPlane()
{
	// create a plane equivelant to the xz plane.  points straight up the y axis.
	this->setPlane( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 1.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 1.0f ) );
}

GEPlane::GEPlane( GEvec3 point1, GEvec3 point2, GEvec3 point3 )
{
	this->setPlane( point1, point2, point3 );
}

// Getters


GEvec3 GEPlane::getNormal() const
{
	return this->normal;
}

GEvec3 GEPlane::getPoint1() const
{
	return this->point1;
}

GEvec3 GEPlane::getPoint2() const
{
	return this->point2;
}

// Functions

void GEPlane::setPlane( GEvec3 a, GEvec3 b, GEvec3 c )
{
	// calculate plane here.

	// to get the normal we need the cross product of two vectors
	GEvec3 ab = b - a;
	GEvec3 ac = c - a;

	GEvec3 pNormal = glm::normalize( glm::cross( ab, ac ) );

	this->normal = pNormal;

	// then use a and b for the points

	this->point1 = a;
	this->point2 = b;
}

GEvec3* GEPlane::intersectRay( GERay ray )
{
	GEvec3* intersectPos = nullptr;

	// we're going to use vector forms of the line and plane to find the intersection

	float numerator = glm::dot( this->getPoint1() - ray.getOrigin(), this->getNormal() );
	float denominator = glm::dot( ray.getDirection(), this->getNormal() );

	// verify that the line is not parallel to plane
	if( denominator != 0 )
	{
		// they are not parallel.  Find scalar value of the line where the plane intercepts it.
		float d = numerator / denominator;

		// now use the scalar to find the coordinates of the intersection

		intersectPos = new GEvec3();

		*intersectPos = ( d * ray.getDirection() ) + ray.getOrigin();
	}
	
	return intersectPos;

}