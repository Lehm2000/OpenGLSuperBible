#ifndef INFOGAMEENGINESETTINGS_H
#define INFOGAMEENGINESETTINGS_H

/**
	InfoGameEngineSettings class
	Place to hold game engine settings
	Author: Jeff Adams
*/

#include "Info.h"
#include "GESetting.h"
#include "GESettingCycleInputToggle.h"
#include "InputItem.h"

// render modes
#define GE_RENDERMODE_FULL		0
#define GE_RENDERMODE_WIRE		1



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
	GESettingCycleInputToggle< unsigned char> renderMode; //unsigned char renderMode;
	GESettingCycleInputToggle< bool > showBoundingBoxes;
	GESettingCycleInputToggle< bool > enableMultiSample;

	// Input Options
	GESettingCycleInputToggle< unsigned char > mouseMode;

public:

	// Structors
	InfoGameEngineSettings();
	InfoGameEngineSettings( const InfoGameEngineSettings& source );

	// Setters and Getters
	void setViewportWidth( const unsigned short viewportWidth );
	unsigned short getViewportWidth() const;

	void setViewportHeight( const unsigned short viewportHeight );
	unsigned short getViewportHeight() const;
	
	void setRenderCam( const std::string renderCam );
	std::string getRenderCam() const;

	void setRenderMode( unsigned char renderMode );
	unsigned char getRenderMode() const;

	void setShowBoundingBoxes( bool showBoundingBoxes );
	bool getShowBoundingBoxes() const;

	unsigned char getMouseMode() const;

	void setEnableMultiSample( bool showBoundingBoxes );
	bool getEnableMultiSample() const;


	// Functions
	
	/**
		ProcessInput
		Function for processing input from the user.  Meant to be stored in the
		inputFunction list as a pointer.  Takes the input state and passes it
		to the Controllable Properties/Settings.
	*/
	virtual void ProcessInput( const GEInputState* inputState );

	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual InfoGameEngineSettings* clone() const;

	virtual std::string getClassName() const;


};

#endif /* INFOGAMEENGINESETTINGS_H */