

#include "GEControllerOscillator.h"

//Structors

GEControllerOscillator::GEControllerOscillator()
{
}

GEControllerOscillator::GEControllerOscillator( glm::vec3 amplitude, float frequency )
{
	this->setAmplitude( amplitude  );
	this->setFrequency( frequency  );
}

GEControllerOscillator::~GEControllerOscillator()
{
}


// Setters
void GEControllerOscillator::setAmplitude( const glm::vec3 amplitude )
{
	this->amplitude = amplitude;
}

void GEControllerOscillator::setFrequency( const float frequency )
{
	this->frequency = frequency / ( 2.0f * 3.1415f );
}


// Getters
glm::vec3 GEControllerOscillator::getAmplitude() const
{
	return this-> amplitude;
}

float GEControllerOscillator::getFrequency() const
{
	return this->frequency * ( 2.0f * 3.1415f );
}


// Functions

void GEControllerOscillator::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	transformedVector = sin( (float)gameTime / frequency ) * amplitude;
}


glm::vec3 GEControllerOscillator::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector + transformedVector;
}
