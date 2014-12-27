#ifndef INFOGAMEVARS_H
#define INFOGAMEVARS_H

/**
	InfoGameVars class
	Place to hold game variables... is this wise??
*/

#include "Info.h"

class InfoGameVars: public Info
{
private:

	// members

	double lastFrameTime;		// in seconds
	double currentFrameTime;	// in seconds

public:

	// Structors
	InfoGameVars();
	InfoGameVars( const InfoGameVars& source );

	// Setters

	
	void setCurrentFrameTime( const double currentFrameTime );

	// Getters

	double getLastFrameTime() const;
	double getCurrentFrameTime() const;

	// Functions
	
	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual InfoGameVars* clone() const;

	double getDeltaFrameTime() const;
	virtual std::string getClassName() const;


};

#endif /* INFOGAMEVARS_H */