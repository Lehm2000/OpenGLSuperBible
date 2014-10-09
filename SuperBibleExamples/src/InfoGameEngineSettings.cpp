
#include "InfoGameEngineSettings.h"

InfoGameEngineSettings::InfoGameEngineSettings()
	:Info()
{
	this->setViewportWidth( 640 );
	this->setViewportHeight( 480 );

	this->setRenderMode( GE_RENDERMODE_FULL );
	this->setShowBoundingBoxes( false );
}

InfoGameEngineSettings::InfoGameEngineSettings( const InfoGameEngineSettings& source )
	:Info( source )
{
	this->setViewportWidth( source.viewportWidth );
	this->setViewportHeight( source.viewportHeight );

	this->setRenderCam( source.renderCam );

	this->setRenderMode( source.renderMode );
	this->setShowBoundingBoxes( source.showBoundingBoxes );
}

// Setters
void InfoGameEngineSettings::setViewportWidth( const unsigned short viewportWidth )
{
	this->viewportWidth = viewportWidth;
}

void InfoGameEngineSettings::setViewportHeight( const unsigned short viewportHeight )
{
	this->viewportHeight = viewportHeight;
}

void InfoGameEngineSettings::setRenderCam( const std::string renderCam )
{
	this->renderCam = renderCam;
}

void InfoGameEngineSettings::setRenderMode( unsigned char renderMode )
{
	this->renderMode = renderMode;
}

void InfoGameEngineSettings::setShowBoundingBoxes( bool showBoundingBoxes )
{
	this->showBoundingBoxes = showBoundingBoxes;
}


// Getters


unsigned short InfoGameEngineSettings::getViewportWidth() const
{
	return this->viewportWidth;
}

unsigned short InfoGameEngineSettings::getViewportHeight() const
{
	return this->viewportHeight;
}

std::string InfoGameEngineSettings::getRenderCam() const
{
	return this->renderCam;
}

unsigned char InfoGameEngineSettings::getRenderMode() const
{
	return this->renderMode;
}

bool InfoGameEngineSettings::getShowBoundingBoxes() const
{
	return this->showBoundingBoxes;
}



// Functions
	
/**
	clone()
	Creates a copy of the object and returns it.
*/
InfoGameEngineSettings* InfoGameEngineSettings::clone() const
{
	return new InfoGameEngineSettings( *this );
}


std::string InfoGameEngineSettings::getClassName() const
{
	return "InfoGameEngineSettings";
}
