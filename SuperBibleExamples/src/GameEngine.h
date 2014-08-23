#ifndef GAMEENGINE_H
#define GAMEENGINE_H

/**
	GameEngine
	Purpose:  This class runs the game
*/

#include <set>

#include "GraphicsEngine.h"
#include "GEObject.h"

class GameEngine
{
private:
	// Members
	
	// Viewport information
	unsigned short viewWidth;
	unsigned short viewHeight;

	// Game Objects
	std::set< GEObject > gameEntities;		// Contains all objects in the game.  Uses the base object class to allow polymorphism.

	// System Objects
	GraphicsEngine* graphics;	// The graphics engine used.  GraphicsEngine will eventually be abstracted for modularity.

public:

	// Structors
	GameEngine();	// Empty Constructor
	GameEngine(const GameEngine& source);	// Copy constructor... not sure if this is actually needed... but just in case
	~GameEngine();	// Destructor.  Also might not be needed because the engine should only be destroyed if the game is exiting.  But for the sake of cleanliness.

	// Setters
	void setViewWidth( const unsigned short viewWidth );
	void setViewHeight( const unsigned short viewHeight );
	void setViewPort( const unsigned short viewWidth, const unsigned short viewHeight );

	// Getters
	unsigned short getViewWidth() const;
	unsigned short getViewHeight() const;
	double getGameTime() const;

	//Functions
	bool Initialize();
	void Update();
	void Render();
	bool isRunning() const;
	
};


#endif /* GAMEENGINE_H */