#ifndef INFOGAMEENGINESETTINGS_H
#define INFOGAMEENGINESETTINGS_H

/**
	InfoGameEngineSettings class
	Place to hold game engine settings
	Author: Jeff Adams
*/

#include "Info.h"

// render modes
#define GE_RENDERMODE_FULL		1
#define GE_RENDERMODE_WIRE		2

class InfoGameEngineSettings: public Info
{
private:

	// members

	// Viewport options
	unsigned short viewportWidth;
	unsigned short viewportHeight;

	std::string renderCam;				// Name of the camera object we are 
		//rendering from.  TODO how to allow more than one render cam?

	// Render options ( That aren't exclusive to a specific render engine )
	unsigned char renderMode;
	bool showBoundingBoxes;

public:

	// Structors
	InfoGameEngineSettings();
	InfoGameEngineSettings( const InfoGameEngineSettings& source );

	// Setters
	void setViewportWidth( const unsigned short viewportWidth );
	void setViewportHeight( const unsigned short viewportHeight );
	
	void setRenderCam( const std::string renderCam );

	void setRenderMode( unsigned char renderMode );
	void setShowBoundingBoxes( bool showBoundingBoxes );

	// Getters
	unsigned short getViewportWidth() const;
	unsigned short getViewportHeight() const;
	
	std::string getRenderCam() const;

	unsigned char getRenderMode() const;
	bool getShowBoundingBoxes() const;


	// Functions
	
	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual InfoGameEngineSettings* clone() const;

	virtual std::string getClassName() const;


};

#endif /* INFOGAMEENGINESETTINGS_H */