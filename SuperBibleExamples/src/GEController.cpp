
#include <glm\glm.hpp>

#include "GEController.h"

// Structors

GEController::GEController()
{
	this->transformedVector = glm::vec3( 0.0f, 0.0f, 0.0f );
}

GEController::GEController( const GEController& source)
{
	this->transformedVector = source.transformedVector;
}

GEController::~GEController()
{
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
