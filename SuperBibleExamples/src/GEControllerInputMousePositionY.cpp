
#include <glm\glm.hpp>

#include "GEControllerInputMousePositionY.h"
#include "GEInputState.h"

GEControllerInputMousePositionY::GEControllerInputMousePositionY()
	:GEControllerConstant()
{
	this->mousePositionY = 0.0f;
	this->mousePositionYPrev = 0.0f;
}

GEControllerInputMousePositionY::GEControllerInputMousePositionY( const glm::vec3 deltaVec )
	:GEControllerConstant( deltaVec )
{
	this->mousePositionY = 0.0f;
	this->mousePositionYPrev = 0.0f;
	
}

GEControllerInputMousePositionY::GEControllerInputMousePositionY( const GEControllerInputMousePositionY& source )
	:GEControllerConstant( source.deltaVec )
{
	this->mousePositionY = source.mousePositionY;
	this->mousePositionYPrev = source.mousePositionYPrev;
}

// Setters

void GEControllerInputMousePositionY::setGameEntities( const std::map< std::string, GEObject* >* gameEntities )
{
	// Overloaded from GEController so that this controller grabs the current mouse position from SYS_Input_State
	
	
	this->gameEntities = gameEntities;

	// once the gameEntities is set... now we can get the mouse position from the input state object.
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->mousePositionY = inputState->getMousePosition().y;
		this->mousePositionYPrev = this->mousePositionY;
	}
}

// Functions

GEControllerInputMousePositionY::~GEControllerInputMousePositionY()
{
}


GEControllerInputMousePositionY* GEControllerInputMousePositionY::clone() const
{
	return new GEControllerInputMousePositionY( *this );
}


void GEControllerInputMousePositionY::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	// Start by updating the mouse position
	this->mousePositionYPrev = this->mousePositionY;

	// get the new mouse position
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->mousePositionY = inputState->getMousePosition().y;
	}

	// find the change
	float mousePosXDelta = this->mousePositionY - this->mousePositionYPrev;

	// apply the change. x = y rotation, y = x rotation
	transformedVector += deltaVec * mousePosXDelta;
}


glm::vec3 GEControllerInputMousePositionY::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector + transformedVector;
}
