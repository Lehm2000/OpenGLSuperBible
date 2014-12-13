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
#include <vector>

#include "MaterialManager.h"
#include "GEObject.h"
#include "GEMesh.h"
#include "GEMaterial.h"
#include "InputItem.h"
#include "MUMesh.h"

// Constants

#define GE_COLLIDE_BOUNDINGBOX			1
#define GE_COLLIDE_BOUNDINGCYLINDER		2
#define GE_COLLIDE_BOUNDINGSPHERE		3
#define GE_COLLIDE_MESH					4

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



class GraphicsEngine
{
protected:
	
	const GEObjectContainer* gameEntities;  // this is a pointer to the gameEntities... passed from the game engine when it was created.
	std::queue< InputItem > inputList;					// Holds inputs that have been recorded.

public:

	// structors
	GraphicsEngine();
	GraphicsEngine( const GEObjectContainer* gameEntities );  // passes a pointer to the game entities.
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
	virtual void RenderTut(const double currentTime) = 0;

	/**
		Renders the scene. 

		@param currentTime - the time since the program started.
		@param gameEntities - pointer to the game Engine entities.
	*/
	virtual void Render(const double currentTime ) = 0;

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
	virtual bool BufferMesh( std::string meshPath, MUMesh* mesh ) = 0;
	virtual bool BufferMaterial( std::string materialPath ) = 0;
	
	virtual void QueueInputItem( InputItem input );

	/**
		SetMouseMode()
		Changes the mouse input mode.
		@param mouseMode - which mode to set.  Member of GE_MOUSEMODE_*
		@return void
	*/
	virtual void SetMouseMode( unsigned char mouseMode ) = 0;

	/**
		Gets the current position of the mouse from the graphics engine.
	*/
	virtual GEvec2 GetMousePosition() const = 0;

	/**
		MouseOver
		Finds what object(s) the mouse is over.  This must be part of the graphics engine as its the only class that has
		access to both the object data and mesh data.
		TODO: investigate if its possible to implement this function in this class so we don't need a different version 
		for each render engine.  i.e. all the mesh types will at least have a bounding box.
		@param findClosest - only returns the closest object the mouse is over if true, false returns everyobject the mouse is over
		@param collisionMode - what type of geometry to collide against for the mesh.
		@return object(s) the mouse is over
	*/
	virtual std::vector<std::string> MouseOver( bool findClosest = true, unsigned char collisionMode = GE_COLLIDE_BOUNDINGBOX ) = 0;
	
	
};

#endif // GRAPHICSENGINE_H
