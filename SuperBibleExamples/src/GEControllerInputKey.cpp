
#include "GEControllerInputKey.h"
#include "InputItem.h"
#include "GEInputState.h"

GEControllerInputKey::GEControllerInputKey()
	:GEControllerConstant()
{
	this->setKey( GE_KEY_UNKNOWN );
	
}

GEControllerInputKey::GEControllerInputKey( const glm::vec3 deltaVec, unsigned short key )
	:GEControllerConstant( deltaVec )
{
	this->setKey( key );
}

GEControllerInputKey::GEControllerInputKey( const GEControllerInputKey& source )
	:GEControllerConstant( source.deltaVec )
{
	this->setKey( source.key );
}

GEControllerInputKey::~GEControllerInputKey()
{
}


// Setters
void GEControllerInputKey::setKey( unsigned short key )
{
	this->key = key;
}


// Getters
unsigned int GEControllerInputKey::getKey() const
{
	return this->key;
}


// Functions

GEControllerInputKey* GEControllerInputKey::clone() const
{
	return new GEControllerInputKey( *this );
}

void GEControllerInputKey::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	// Update the key status

	this->pressedPrev = this->pressed;

	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find( "SYS_Input_State" );
	
	if ( isIt != gameEntities->end() )
	{
		GEInputState* inputState = (GEInputState*)isIt->second;
		this->pressed = inputState->getKeyboardKey( this->key );
	}
	else
	{
		// if fail to find the input state object... assume its not pressed.
		this->pressed = false;
	}

	// then apply the delta if the key is pressed.
	if ( this->pressed )
	{
		transformedVector = transformedVector + ( deltaVec * (float)deltaTime );
	}
}

glm::vec3 GEControllerInputKey::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector + transformedVector;
}
