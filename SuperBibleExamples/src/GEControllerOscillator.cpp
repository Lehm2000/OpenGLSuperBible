

#include "GEControllerOscillator.h"
#include "GEConstants.h"

//Structors

GEControllerOscillator::GEControllerOscillator()
{
}

GEControllerOscillator::GEControllerOscillator( glm::vec3 amplitude, float frequency )
	:GEController()
{
	this->setAmplitude( amplitude  );
	this->setFrequency( frequency );
}

GEControllerOscillator::GEControllerOscillator( const GEControllerOscillator& source )
	:GEController( source.parent, source.gameEntities )
{
	this->setAmplitude( source.amplitude );
	this->frequency = source.frequency;	// can't use setter for this as it modifies incoming value.
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
	this->frequency = frequency / ( 2.0f * GE_PI );
}


// Getters
glm::vec3 GEControllerOscillator::getAmplitude() const
{
	return this-> amplitude;
}

float GEControllerOscillator::getFrequency() const
{
	return this->frequency * ( 2.0f * GE_PI );
}


// Functions

GEControllerOscillator* GEControllerOscillator::clone() const
{
	return new GEControllerOscillator( *this );
}

void GEControllerOscillator::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	transformedVector = sin( (float)gameTime / frequency ) * amplitude;
}


glm::vec3 GEControllerOscillator::CalcTransform( glm::vec3 sourceVector )
{
	return sourceVector + transformedVector;
}
