#include "InfoGameVars.h"


// Structors
InfoGameVars::InfoGameVars()
{
	this->lastFrameTime = 0.0 ;
	this->setCurrentFrameTime( 0.0 );
}

InfoGameVars::InfoGameVars( const InfoGameVars& source )
	:Info( source )
{
	this->lastFrameTime = source.lastFrameTime;
	this->currentFrameTime = currentFrameTime;
}

// Setters



void InfoGameVars::setCurrentFrameTime( const double currentFrameTime )
{
	this->lastFrameTime = this->currentFrameTime;
	this->currentFrameTime = currentFrameTime;
}

// Getters

double InfoGameVars::getLastFrameTime() const
{
	return this->lastFrameTime;
}

double InfoGameVars::getCurrentFrameTime() const
{
	return this->currentFrameTime;
}

// Functions

InfoGameVars* InfoGameVars::clone() const
{
	return new InfoGameVars( *this );
}

double InfoGameVars::getDeltaFrameTime() const
{
	return currentFrameTime - lastFrameTime;
}

std::string InfoGameVars::getClassName() const
{
	return "InfoGameVars";
}