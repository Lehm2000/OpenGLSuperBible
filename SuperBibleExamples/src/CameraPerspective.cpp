
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
float CameraPerspective::getBaseFov() const
{
	return fov.getBaseValue();
}

float CameraPerspective::getFinalFov() const
{
	return fov.getFinalValue();
}

// Functions
std::string CameraPerspective::getClassName()
{
	return "CameraPerspective";
}

void CameraPerspective::Update( const double gameTime, const double deltaTime)
{
	// Let property controllers do their thing.

	position.Update( gameTime, deltaTime);
	rotation.Update( gameTime, deltaTime);
	scale.Update( gameTime, deltaTime);
	fov.Update( gameTime, deltaTime);
}

void CameraPerspective::addFOVController( GEControllerf1* scaleController)
{
	fov.addController( scaleController, this );
}

void CameraPerspective::removeFOVController( const unsigned int index )
{
	this->fov.removeController( index );
}