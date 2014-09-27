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
#include "GEObjectContainer.h"

class GameEngine
{
private:
	// Members
	
	// Time Information
	double lastFrameTime;  

	// Game Objects
	GEObjectContainer gameEntities;		// Contains all objects in the game.

	// System Objects
	GraphicsEngine* graphics;	// The graphics engine used.  GraphicsEngine will eventually be abstracted for modularity.

public:

	// Structors
	GameEngine();	// Empty Constructor
	GameEngine(const GameEngine& source);	// Copy constructor... not sure if this is actually needed... but just in case
	~GameEngine();	// Destructor.  Also might not be needed because the engine should only be destroyed if the game is exiting.  But for the sake of cleanliness.

	// Setters
	
	// Getters
	double getGameTime() const;

	//Functions
	bool Initialize();
	void Update();
	void Render();
	bool isRunning() const;

	/**
		AddEntity()
		Adds an object/entity to the gameEntities map
		@param entityName - key for map so that the object can be referenced later.
		@param entity - a pointer to the object itself.
		@return - if successful
	*/
	bool AddEntity( const std::string entityName, GEObject* entity);

	/**
		RemoveEntity()
		Removes an object/entity from the gameEntities map
		@param entityName  - key for the object in the map to be removed
	*/
	void RemoveEntity( const std::string entityName);

	/**
		GetEntity()
		Returns a pointer to object specified by entityName
		@param entityName - name of object to return
		@return - the pointer to the object, or nullptr if not found
	*/
	GEObject* GetEntity( const std::string entityName );

	bool LoadMesh( std::string meshPath );
	bool LoadMaterial( std::string materialPath );
	void FillGEVertex( GEVertex* dest, float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float u, float v );
	
};


#endif /* GAMEENGINE_H */