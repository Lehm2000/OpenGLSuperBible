
#include <glm\glm.hpp>

#include "PerspectiveCamera.h"

// Structors
PerspectiveCamera::PerspectiveCamera()
{
	this->setFov( 45.0f );
}

PerspectiveCamera::PerspectiveCamera( glm::vec3 position, glm::vec3 rotation, float fov, bool targeted, glm::vec3 targetPosition )
	:CameraObject( position, rotation, targeted, targetPosition)
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

// Functions
std::string PerspectiveCamera::getClassName()
{
	return "PerspectiveCamera";
}