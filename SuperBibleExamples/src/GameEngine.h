#ifndef GAMEENGINE_H
#define GAMEENGINE_H

/**
	GameEngine
	Purpose:  This class runs the game
*/

#include <vector>
#include <string>
#include <map>
#include <glm\glm.hpp>

#include "GraphicsEngine.h"
#include "GEObject.h"
#include "CameraObject.h"

class GameEngine
{
private:
	// Members
	
	// Viewport information
	unsigned short viewWidth;
	unsigned short viewHeight;

	// Game Objects
	CameraObject* gameCam;					// The game camera.  TODO: Should this be in gameEntities?
	std::map< std::string, GEObject > gameEntities;		// Contains all objects in the game.  Uses the base object class to allow polymorphism.

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
	void CreateGameCam( const char camType, glm::vec3 position, glm::vec3 rotation, float fov, glm::vec3 targetPosition = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	void DestroyGameCam();
	bool Initialize();
	void Update();
	void Render();
	bool isRunning() const;

	bool AddEntity( const std::string entityName, GEObject entity);
	void RemoveEntity( const std::string entityName);

	bool LoadMesh( std::string meshPath );
	void FillGEVertex( GEVertex* dest, float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float u, float v );
	
};


#endif /* GAMEENGINE_H */