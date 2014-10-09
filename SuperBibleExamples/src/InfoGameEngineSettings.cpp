
#include "InfoGameEngineSettings.h"

InfoGameEngineSettings::InfoGameEngineSettings()
	:Info()
{
	
	this->setRenderMode( GE_RENDERMODE_FULL );
	this->setShowBoundingBoxes( false );
}

InfoGameEngineSettings::InfoGameEngineSettings( const InfoGameEngineSettings& source )
	:Info( source )
{
	this->setRenderMode( source.renderMode );
	this->setShowBoundingBoxes( source.showBoundingBoxes );
}

// Setters
void InfoGameEngineSettings::setRenderMode( unsigned char renderMode )
{
	this->renderMode = renderMode;
}

void InfoGameEngineSettings::setShowBoundingBoxes( bool showBoundingBoxes )
{
	this->showBoundingBoxes = showBoundingBoxes;
}


// Getters
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
