
#include "InfoGameEngineSettings.h"

InfoGameEngineSettings::InfoGameEngineSettings()
	:Info()
{
	this->setViewportWidth( 640 );
	this->setViewportHeight( 480 );

	// Initialize Settings here
	this->renderMode.setValueIndex( 0 );
	std::vector< unsigned char > renderModeList;
	renderModeList.push_back( GE_RENDERMODE_FULL );
	renderModeList.push_back( GE_RENDERMODE_WIRE );
	this->renderMode.setValueList( renderModeList );
	this->renderMode.setEngineAction( GE_ENGINE_ACTION_CHANGERENDERMODE );
	this->renderMode.setAction( GE_ACTION_PRESS	);

	this->showBoundingBoxes.setValueIndex( 0 );
	std::vector< bool > showBoundingList;
	showBoundingList.push_back( false );	// best to have only two options for bools.
	showBoundingList.push_back( true );
	this->showBoundingBoxes.setValueList( showBoundingList );
	this->showBoundingBoxes.setEngineAction( GE_ENGINE_ACTION_TOGGLEBOUNDINGBOX );
	this->showBoundingBoxes.setAction( GE_ACTION_PRESS );

	this->enableMultiSample.setValueIndex( 0 );
	std::vector< bool > enableMultiSampleList;
	enableMultiSampleList.push_back( false );
	enableMultiSampleList.push_back( true );
	this->enableMultiSample.setValueList( enableMultiSampleList );
	this->enableMultiSample.setEngineAction( GE_ENGINE_ACTION_TOGGLEMULTISAMPLE );
	this->enableMultiSample.setAction( GE_ACTION_PRESS );

	this->mouseMode.setValueIndex( 0 );
	std::vector< unsigned char > mouseModeList;
	mouseModeList.push_back( GE_MOUSEMODE_LOOK );
	mouseModeList.push_back( GE_MOUSEMODE_POINT );
	this->mouseMode.setValueList( mouseModeList );
	this->mouseMode.setEngineAction( GE_ENGINE_ACTION_CHANGECURSORMODE );
	this->mouseMode.setAction( GE_ACTION_PRESS );

}

InfoGameEngineSettings::InfoGameEngineSettings( const InfoGameEngineSettings& source )
	:Info( source )
{
	this->setViewportWidth( source.viewportWidth );
	this->setViewportHeight( source.viewportHeight );

	this->setRenderCam( source.renderCam );

	this->renderMode = source.renderMode;
	this->showBoundingBoxes = source.showBoundingBoxes;
	this->enableMultiSample = source.enableMultiSample;
}

// Setters and Getters
void InfoGameEngineSettings::setViewportWidth( const unsigned short viewportWidth )
{
	this->viewportWidth = viewportWidth;
}

unsigned short InfoGameEngineSettings::getViewportWidth() const
{
	return this->viewportWidth;
}

void InfoGameEngineSettings::setViewportHeight( const unsigned short viewportHeight )
{
	this->viewportHeight = viewportHeight;
}

unsigned short InfoGameEngineSettings::getViewportHeight() const
{
	return this->viewportHeight;
}

void InfoGameEngineSettings::setRenderCam( const std::string renderCam )
{
	this->renderCam = renderCam;
}

std::string InfoGameEngineSettings::getRenderCam() const
{
	return this->renderCam;
}

void InfoGameEngineSettings::setRenderMode( unsigned char renderMode )
{
	this->renderMode.setValue( renderMode );
}

unsigned char InfoGameEngineSettings::getRenderMode() const
{
	return this->renderMode.getValue();
}

void InfoGameEngineSettings::setShowBoundingBoxes( bool showBoundingBoxes )
{
	this->showBoundingBoxes.setValue( showBoundingBoxes );
}

bool InfoGameEngineSettings::getShowBoundingBoxes() const
{
	return this->showBoundingBoxes.getValue();
}

unsigned char InfoGameEngineSettings::getMouseMode() const
{
	return this->mouseMode.getValue();
}

void InfoGameEngineSettings::setEnableMultiSample( bool showBoundingBoxes )
{
	this->enableMultiSample.setValue( showBoundingBoxes );
}

bool InfoGameEngineSettings::getEnableMultiSample() const
{
	return this->enableMultiSample.getValue();
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
	showBoundingBoxes.ProcessInput( inputState );
	enableMultiSample.ProcessInput( inputState );

	mouseMode.ProcessInput( inputState );
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

