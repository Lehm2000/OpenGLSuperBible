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

	// Render options ( That aren't exclusive to a specific render engine )
	unsigned char renderMode;
	bool showBoundingBoxes;

public:

	// Structors
	InfoGameEngineSettings();
	InfoGameEngineSettings( const InfoGameEngineSettings& source );

	// Setters
	void setRenderMode( unsigned char renderMode );
	void setShowBoundingBoxes( bool showBoundingBoxes );

	// Getters
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