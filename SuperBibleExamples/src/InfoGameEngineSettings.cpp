
#include "InfoGameEngineSettings.h"

InfoGameEngineSettings::InfoGameEngineSettings()
	:Info()
{
	this->setViewportWidth( 640 );
	this->setViewportHeight( 480 );

	// Initialize Settings here
	this->renderMode.setValueIndex( 0 );
	std::vector< unsigned char> modeList;
	modeList.push_back( GE_RENDERMODE_FULL );
	modeList.push_back( GE_RENDERMODE_WIRE );
	this->renderMode.setValueList( modeList );
	this->renderMode.setEngineAction( GE_ENGINE_ACTION_CHANGERENDERMODE );
	this->renderMode.setAction( GE_ACTION_PRESS	);
	

	//this->setRenderMode( GE_RENDERMODE_FULL );
	this->setShowBoundingBoxes( false );
}

InfoGameEngineSettings::InfoGameEngineSettings( const InfoGameEngineSettings& source )
	:Info( source )
{
	this->setViewportWidth( source.viewportWidth );
	this->setViewportHeight( source.viewportHeight );

	this->setRenderCam( source.renderCam );

	this->renderMode = renderMode;
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
	//this->renderMode = renderMode;
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
	return this->renderMode.getValue();
}

bool InfoGameEngineSettings::getShowBoundingBoxes() const
{
	return this->showBoundingBoxes;
}



// Functions
	
void InfoGameEngineSettings::ProcessInput( const GEInputState* inputState)
{
	// pass it on to the controllers to do their thing.
	position.ProcessInput( inputState );
	rotation.ProcessInput( inputState );
	scale.ProcessInput( inputState );

	// pass it on to the settings
	renderMode.ProcessInput( inputState );
}

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

