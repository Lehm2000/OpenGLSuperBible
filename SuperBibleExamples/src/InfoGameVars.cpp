#include "InfoGameVars.h"


// Structors
InfoGameVars::InfoGameVars()
{
	lastFrameTime = 0.0 ;
	this->setCurrentFrameTime( 0.0 );
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

double InfoGameVars::getDeltaFrameTime() const
{
	return currentFrameTime - lastFrameTime;
}

std::string InfoGameVars::getClassName()
{
	return "InfoGameVars";
}