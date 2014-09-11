
#include <glm\glm.hpp>

#include "CameraPerspective.h"

// Structors
CameraPerspective::CameraPerspective()
{
	this->setFov( 45.0f );
}

CameraPerspective::CameraPerspective( glm::vec3 position, glm::vec3 rotation, float fov )
	:CameraObject( position, rotation )
{
	this->setFov( fov );
}

CameraPerspective::~CameraPerspective()
{
}

// Setters
void CameraPerspective::setFov( const float fov )
{
	this->fov = fov;
}

// Getters
float CameraPerspective::getFov() const
{
	return fov;
}

// Functions
std::string CameraPerspective::getClassName()
{
	return "CameraPerspective";
}