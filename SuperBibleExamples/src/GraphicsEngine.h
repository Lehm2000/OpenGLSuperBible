#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

/**
	GraphicsEngine Class
	GraphicsEngine.cpp
	Purpose: This is the base class for the graphics engine.  This should be a virtual class to be inherited by the specific graphics engines.
			
	@author Jeff Adams
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <map>
#include <queue>

#include "MaterialManager.h"
#include "GEObject.h"
#include "GEMesh.h"
#include "GEMaterial.h"
#include "InputItem.h"

//temporary place to define vertex structs
struct vertex
{
	// Position
	float x;
	float y;
	float z;
	float w;

	// Color
	float r;
	float g;
	float b;
	float a;

};

struct GEVertex
{
	// Position
	float x;
	float y;
	float z;
	//float w; // needed?

	// Color
	float r;
	float g;
	float b;
	float a;

	// Normal
	float nx;
	float ny;
	float nz;

	// Texture  // how do we do more than one texture channel.
	float u;
	float v;
};

class GraphicsEngine
{
protected:
	
	const std::map< std::string, GEObject* >* gameEntities;  // this is a pointer to the gameEntities... passed from the game engine when it was created.
	std::queue< InputItem > inputList;					// Holds inputs that have been recorded.

public:

	// structors
	GraphicsEngine();
	GraphicsEngine( const std::map< std::string, GEObject* >* gameEntities );  // passes a pointer to the game entities.
	virtual ~GraphicsEngine();

	// setters

	// getters
	virtual std::queue< InputItem >* getInputList();

	//functions...
	
	/**
		Cleans up resources allocated by the Graphics Engine

		@return If successful
	*/
	virtual bool CleanUp() = 0;

	/**
		Checks if window should close

		@return If successful
	*/
	virtual int CheckWindowClose() = 0;

	/**
		Initialize the Engine

		@return If successfully initialized
	*/
	virtual bool Init() = 0;

	/**
		Renders the scene.  This version is for tutorial code.

		@param currentTime - the time since the program started.
	*/
	virtual void Render(const double currentTime) = 0;

	/**
		Renders the scene. 

		@param currentTime - the time since the program started.
		@param gameEntities - pointer to the game Engine entities.
	*/
	virtual void Render(const double currentTime, const std::map< std::string, GEObject* >* gameEntities) = 0;

	/**
		Updates the window size.  

		@param x
		@param y
		@param width
		@param height
	*/
	virtual void UpdateWindowSize(int x, int y, int width, int height) = 0;
	
	//Getters

	/**
		Time since program started.  

		@return time in seconds since the program started.
	*/
	virtual double getCurrentTime() const = 0;

	virtual bool isMeshBuffered( std::string meshPath ) = 0;
	virtual bool isMaterialBuffered( std::string materialPath ) = 0;

	/**
		BufferMesh()
		Takes mesh and stores it in the map of mesh objects
		@param meshPath - unique mesh name/path.
		@param mesh - pointer to vertex data
		@param numVerts - number of vertices in the mesh.
		@param vertIndices - pointer to vertex indices
		@param numIndices
	*/
	virtual bool BufferMesh( std::string meshPath, GEVertex* mesh, unsigned int numVerts, unsigned int* vertIndices, unsigned int numIndices ) = 0;
	virtual bool BufferMaterial( std::string materialPath ) = 0;
	
	virtual void QueueInputItem( InputItem input );
	
	
};

#endif // GRAPHICSENGINE_H
