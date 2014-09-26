


#include "InfoViewport.h"

// Structors
InfoViewport::InfoViewport()
{
	this->setVisible(false);
}

InfoViewport::InfoViewport( unsigned short viewportWidth, unsigned short viewportHeight )
{
	this->setVisible(false);

	this->setViewportWidth( viewportWidth );
	this->setViewportHeight( viewportHeight );
}

// Setters

void InfoViewport::setViewportWidth( const unsigned short viewportWidth )
{
	this->viewportWidth = viewportWidth;
}

void InfoViewport::setViewportHeight( const unsigned short viewportHeight )
{
	this->viewportHeight = viewportHeight;
}

void InfoViewport::setRenderCam( const std::string renderCam )
{
	this->renderCam = renderCam;
}

// Getters

unsigned short InfoViewport::getViewportWidth() const
{
	return this->viewportWidth;
}

unsigned short InfoViewport::getViewportHeight() const
{
	return this->viewportHeight;
}

std::string InfoViewport::getRenderCam() const
{
	return this->renderCam;
}

// Functions
std::string InfoViewport::getClassName()
{
	return "ViewportInfo";
}