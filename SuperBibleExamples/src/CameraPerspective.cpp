
#include <glm\glm.hpp>

#include "CameraPerspective.h"
#include "TypeDefinitions.h"

// Structors
CameraPerspective::CameraPerspective()
{
	this->setFov( 45.0f );
}

CameraPerspective::CameraPerspective( GEvec3 position, GEvec3 rotation, float fov )
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
	this->fov.setValue( fov );
}

// Getters
float CameraPerspective::getBaseFov() const
{
	return fov.getBaseValue();
}

float CameraPerspective::getFinalFov() const
{
	float tempVal = fov.getFinalValue();
	return fov.getFinalValue();
}

GEPropertyf1* CameraPerspective::getFOV()
{
	return &fov;
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

void CameraPerspective::setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities)
{
	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
	fov.setControllerGameEntitiesPointer( gameEntities );
}