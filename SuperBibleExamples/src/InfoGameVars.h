#ifndef INFOGAMEVARS_H
#define INFOGAMEVARS_H

/**
	InfoGameVars class
	Place to hold game variables... is this wise??
*/

#include "GEObject.h"

class InfoGameVars: public GEObject
{
private:

	// members

	double lastFrameTime;		// in seconds
	double currentFrameTime;	// in seconds

public:

	// Structors
	InfoGameVars();

	// Setters

	
	void setCurrentFrameTime( const double currentFrameTime );

	// Getters

	double getLastFrameTime() const;
	double getCurrentFrameTime() const;

	// Functions
	
	double getDeltaFrameTime() const;
	virtual std::string getClassName();


};

#endif /* INFOGAMEVARS_H */