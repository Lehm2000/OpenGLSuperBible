
#include "GERay.h"
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>


GERay::GERay()
{
	this->setOrigin( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->setDirection( GEvec3( 0.0f, 0.0f, 0.0f ) );
}

GERay::GERay( GEvec3 origin, GEvec3 direction )
{
	this->setOrigin( origin );
	this->setDirection( direction );
}

// copy if needed
// destructor if needed

// Getters and Setters
void GERay::setOrigin( const GEvec3 origin )
{
	this->origin = origin;
}

GEvec3 GERay::getOrigin() const
{
	return this->origin;
}


void GERay::setDirection( const GEvec3 direction )
{
	this->direction = direction;
}

GEvec3 GERay::getDirection() const
{
	return this->direction;
}

GERay GERay::operator*( glm::mat4& matrix )
{
	GERay transformedRay;
	
	GEvec4 transformedOrigin = matrix * glm::vec4( origin, 1.0f ) ;
	GEvec4 transformedDirection = matrix * glm::vec4( direction, 1.0f ) ;

	transformedRay.setOrigin( GEvec3( transformedOrigin.x, transformedOrigin.y, transformedOrigin.z ) );
	transformedRay.setDirection( GEvec3( transformedDirection.x, transformedDirection.y, transformedDirection.z ) );

	return transformedRay;
}
