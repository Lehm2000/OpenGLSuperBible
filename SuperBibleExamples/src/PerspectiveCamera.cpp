

#include "PerspectiveCamera.h"

// Structors
PerspectiveCamera::PerspectiveCamera()
{
	this->setFov( 45.0f );
}

PerspectiveCamera::PerspectiveCamera( vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, float fov, bool targeted, vmath::vec3 targetPosition )
	:CameraObject( position, rotation, scale, targeted, targetPosition)
{
	this->setFov( fov );
}

PerspectiveCamera::~PerspectiveCamera()
{
}

// Setters
void PerspectiveCamera::setFov( const float fov )
{
	this->fov = fov;
}

// Getters
float PerspectiveCamera::getFov() const
{
	return fov;
}