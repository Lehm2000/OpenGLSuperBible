
#include <glm\glm.hpp>

#include "GEControllerConstant.h"


GEControllerConstant::GEControllerConstant()
{
}

GEControllerConstant::GEControllerConstant(glm::vec3 deltaVec)
{
	this->setDeltaVec( deltaVec );
}

GEControllerConstant::~GEControllerConstant()
{
}

// Setters
void GEControllerConstant::setDeltaVec( const glm::vec3 deltaVec )
{
	this->deltaVec = deltaVec;
}

// Getters
glm::vec3 GEControllerConstant::getDeltaVec() const
{
	return this->deltaVec;
}

// Functions

void GEControllerConstant::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	transformedVector = transformedVector + ( deltaVec * (float)deltaTime );
}

glm::vec3 GEControllerConstant::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector + transformedVector;
}