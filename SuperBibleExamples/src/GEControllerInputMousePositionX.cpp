
#include <glm\glm.hpp>

#include "GEControllerInputMousePositionX.h"
#include "GEInputState.h"

GEControllerInputMousePositionX::GEControllerInputMousePositionX()
	:GEControllerConstant()
{
	this->mousePositionX = 0.0f;
	this->mousePositionXPrev = 0.0f;
}

GEControllerInputMousePositionX::GEControllerInputMousePositionX( const glm::vec3 deltaVec )
	:GEControllerConstant( deltaVec )
{
	this->mousePositionX = 0.0f;
	this->mousePositionXPrev = 0.0f;
	
}

GEControllerInputMousePositionX::GEControllerInputMousePositionX( const GEControllerInputMousePositionX& source )
	:GEControllerConstant( source.deltaVec )
{
	this->mousePositionX = source.mousePositionX;
	this->mousePositionXPrev = source.mousePositionXPrev;
}

// Setters

void GEControllerInputMousePositionX::setGameEntities( const std::map< std::string, GEObject* >* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;

	// once the gameEntities is set... now we can get the mouse position from the input state object.
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->mousePositionX = inputState->getMousePosition().x;
		this->mousePositionXPrev = this->mousePositionX;
	}
}

// Functions

GEControllerInputMousePositionX::~GEControllerInputMousePositionX()
{
}


GEControllerInputMousePositionX* GEControllerInputMousePositionX::clone() const
{
	return new GEControllerInputMousePositionX( *this );
}


void GEControllerInputMousePositionX::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	// Start by updating the mouse position
	this->mousePositionXPrev = this->mousePositionX;

	// get the new mouse position
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->mousePositionX = inputState->getMousePosition().x;
	}

	// find the change
	float mousePosXDelta = this->mousePositionX - this->mousePositionXPrev;

	// apply the change. x = y rotation, y = x rotation
	transformedVector += deltaVec * mousePosXDelta;
}


glm::vec3 GEControllerInputMousePositionX::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector + transformedVector;
}
