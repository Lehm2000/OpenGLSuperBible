
#include <glm\glm.hpp>

#include "CameraPerspective.h"
#include "TypeDefinitions.h"

// Structors
CameraPerspective::CameraPerspective()
{
	this->setFovStart( 45.0f );
}

CameraPerspective::CameraPerspective( GEvec3 position, GEvec3 rotation, float fov )
	:CameraObject( position, rotation )
{
	this->setFovStart( fov );
}

CameraPerspective::CameraPerspective( const CameraPerspective& source )
	:CameraObject( source )
{
	this->fov = source.fov;
}

CameraPerspective::~CameraPerspective()
{
}

// Setters


// Getters

// Functions
std::string CameraPerspective::getClassName() const
{
	return "CameraPerspective";
}

void CameraPerspective::Update( const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime)
{
	// Let property controllers do their thing.
	CameraObject::Update( gameEntities, gameTime, deltaTime); // update the parent first.

	fov.Update( this, gameEntities, gameTime, deltaTime);  
}

void CameraPerspective::ProcessInput( const GEInputState* inputState)
{
	// pass it on to the controllers to do their thing.
	CameraObject::ProcessInput( inputState );  // send it to the parent object first
	
	fov.ProcessInput( inputState );
}

void CameraPerspective::addFovController( GEControllerf1* fovController)
{
	this->fov.addController( fovController );
}

void CameraPerspective::removeFovController( const unsigned int index )
{
	this->fov.removeController( index );
}

CameraPerspective* CameraPerspective::clone() const
{
	return new CameraPerspective( *this );
}

/*
void CameraPerspective::setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities)
{
	this->setGameEntities( gameEntities );

	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
	fov.setControllerGameEntitiesPointer( gameEntities );
}
*/