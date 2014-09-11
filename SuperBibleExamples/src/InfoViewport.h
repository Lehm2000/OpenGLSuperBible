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

public:
	// Structors
	InfoViewport();
	InfoViewport( unsigned short viewportWidth, unsigned short viewportHeight );
	// destructor not needed yet

	// Setters
	void setViewportWidth( const unsigned short viewportWidth );
	void setViewportHeight( const unsigned short viewportHeight );

	// Getters
	unsigned short getViewportWidth() const;
	unsigned short getViewportHeight() const;

	// Functions
	virtual std::string getClassName();
};



#endif /* VIEWPORTINFO_H */