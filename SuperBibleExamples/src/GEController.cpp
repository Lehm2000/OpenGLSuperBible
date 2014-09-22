
#include <glm\glm.hpp>

#include "GEController.h"

// Structors

GEController::GEController()
{
	this->transformedVector = glm::vec3( 0.0f, 0.0f, 0.0f );

	this->setParent( nullptr );
	this->setGameEntities( nullptr );
}

GEController::GEController( const GEObject* parent, const std::map< std::string, GEObject* >* gameEntities )
{
	this->transformedVector = glm::vec3( 0.0f, 0.0f, 0.0f );

	this->setParent( parent );
	this->setGameEntities( gameEntities );
}

GEController::GEController( const GEController& source)
{
	this->transformedVector = source.transformedVector;
	this->setParent( source.parent );	// okay to pass this pointer along
	this->setGameEntities( source.gameEntities );	// okay to pass this pointer along
}

GEController::~GEController()
{
	// DO NOT DELETE parent and gameEntities... I know you want to because they are pointers... trust me on this one, just don't.
}

// Setters

void GEController::setParent( const GEObject* parent )
{
	this->parent = parent;
}

void GEController::setGameEntities( const std::map< std::string, GEObject* >* gameEntities )
{
	this->gameEntities = gameEntities;
}


// Getters

const GEObject* GEController::getParent() const
{
	return this->parent;
}

const std::map< std::string, GEObject* >* GEController::getGameEntities() const
{
	return this->gameEntities;
}


// Functions

GEController* GEController::clone() const
{
	return new GEController(*this);
}

void GEController::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	this->transformedVector = glm::vec3( 0.0f, 0.0f, 0.0f );  // completely ignore incoming data.
}

glm::vec3 GEController::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector;  //return the source as the transformed.
}
