


#include "ViewportInfo.h"

// Structors
ViewportInfo::ViewportInfo()
{
	this->setVisible(false);
}

ViewportInfo::ViewportInfo( unsigned short viewportWidth, unsigned short viewportHeight )
{
	this->setVisible(false);

	this->setViewportWidth( viewportWidth );
	this->setViewportHeight( viewportHeight );
}

// Setters
void ViewportInfo::setViewportWidth( const unsigned short viewportWidth )
{
	this->viewportWidth = viewportWidth;
}

void ViewportInfo::setViewportHeight( const unsigned short viewportHeight )
{
	this->viewportHeight = viewportHeight;
}

// Getters
unsigned short ViewportInfo::getViewportWidth() const
{
	return this->viewportWidth;
}

unsigned short ViewportInfo::getViewportHeight() const
{
	return this->viewportHeight;
}

// Functions
std::string ViewportInfo::getClassName()
{
	return "ViewportInfo";
}