#ifndef VIEWPORTINFO_H
#define VIEWPORTINFO_H


/**
	ViewportInfo Class
	Author: Jeff Adams
	This class provides all the options for the viewport/window.  Extension of the GEObject class so that it can be passed to the render as a gameEntity.
*/

#include "GEObject.h"

class InfoViewport: public GEObject
{
private:
	// Members
	unsigned short viewportWidth;
	unsigned short viewportHeight;
	std::string renderCam;				// Name of the camera object we are 
		//rendering from.  TODO might want to move somewhere else.  This isn't
		//explicitly a viewport setting.  Also might need to allow more than
		//one render cam.

public:
	// Structors
	InfoViewport();
	InfoViewport( unsigned short viewportWidth, unsigned short viewportHeight );
	// destructor not needed yet

	// Setters
	void setViewportWidth( const unsigned short viewportWidth );
	void setViewportHeight( const unsigned short viewportHeight );
	void setRenderCam( const std::string renderCam );

	// Getters
	unsigned short getViewportWidth() const;
	unsigned short getViewportHeight() const;
	std::string getRenderCam() const;

	// Functions
	virtual std::string getClassName();
};



#endif /* VIEWPORTINFO_H */