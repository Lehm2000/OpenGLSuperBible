#ifndef GRAPHICSENGINEOPENGL_H
#define GRAPHICSENGINEOPENGL_H

/**
	GraphicsEngine Class
	GraphicsEngine.cpp
	Purpose: Graphics engine for testing OpenGL.
			
	@author Jeff Adams
*/

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <map>
#include <queue>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glext.h"

#include "GraphicsEngine.h"
#include "MaterialManager.h"
#include "TextureManager.h"
#include "GEObject.h"
#include "GEObjectContainer.h"
#include "GEResourceContainer.h"
#include "GEMesh.h"
#include "GEMaterial.h"
#include "InputItem.h"
#include "GEConstants.h"


class GraphicsEngineOpenGL: public GraphicsEngine
{
private:
	GLFWwindow* window;
				
	GEResourceContainer< GEMesh> resMesh;				// Holds all the mesh information for the Graphics Engine.
	GEResourceContainer< GEMaterial> resMaterial;		// Holds all materials for the game.  Materials hold the shader plus references to the textures used.			
	GEResourceContainer< GLuint > resTexture;			// Holds textures for the game.
	
	MaterialManager materialMan;	// for material operations
	TextureManager textureMan;		// for texture operations

	// Chapter 09 framebuffer object tutorial
	GLuint fbo;
	GLuint color_texture, depth_texture;

public:

	

	// structors
	GraphicsEngineOpenGL();
	GraphicsEngineOpenGL( const GEObjectContainer* gameEntities );  // passes a pointer to the game entities.
	~GraphicsEngineOpenGL();

	// setters

	// getters

	//functions...
	
	/**
		Cleans up resources allocated by the Graphics Engine

		@return If successful
	*/
	bool CleanUp();

	/**
		Checks if window should close

		@return If successful
	*/
	int CheckWindowClose();

	/**
		Initialize the Engine

		@return If successfully initialized
	*/
	bool Init();

	/**
		Initialize the Shaders.  Currently only initializes one and returns program.  

		@return success
	*/
	bool InitShaders(void);

	/**
		Initialize the geometry buffers  
	*/
	void InitBuffers(void);

	/**
		Initialze/load the textures  
	*/
	void InitTextures(void);

	/**
		Renders the scene.  This version is for tutorial code.

		@param currentTime - the time since the program started.
	*/
	void RenderTut(const double currentTime);

	/**
		Renders the scene. 

		@param currentTime - the time since the program started.
		@param gameEntities - pointer to the game Engine entities.
	*/
	void Render(const double currentTime );

	/**
		Renders the FPS counter. 

		@param currentTime - the time since the program started.
		
	*/
	void RenderFPS(const double currentTime);

	/**
		Updates the window size.  

		@param x
		@param y
		@param width
		@param height
	*/
	void UpdateWindowSize(int x, int y, int width, int height);
	
	//Getters

	/**
		Time since program started.  

		@return time in seconds since the program started.
	*/
	double getCurrentTime() const;

	bool isMeshBuffered( std::string meshPath );
	bool isMaterialBuffered( std::string materialPath );
	bool isTextureBuffered( std::string texturePath );

	/**
		BufferMesh()
		Takes mesh and stores it in the map of mesh objects... TODO should
		this take just the path like the BufferMaterial does?
		@param meshPath - unique mesh name/path.
		@param mesh - pointer to vertex data
		@param numVerts - number of vertices in the mesh.
		@param vertIndices - pointer to vertex indices
		@param numIndices
	*/
	bool BufferMesh( std::string meshPath, MUMesh* mesh );
	bool BufferMaterial( std::string materialPath );
	bool BufferTexture( std::string texturePath );

	/**
		SetMouseMode()
		Changes the mouse input mode.
		@param mouseMode - which mode to set.  Member of GE_MOUSEMODE_*
		@return void
	*/
	virtual void SetMouseMode( unsigned char mouseMode );

	/**
		Gets the current position of the mouse from the graphics engine.
	*/
	virtual GEvec2 GetMousePosition() const;

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
	virtual std::vector<std::string> MouseOver( bool findClosest = true, unsigned char collisionMode = GE_COLLIDE_BOUNDINGBOX );
	
	// Callback Functions

	static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// since this function is not a part of this class officially we need to 
		// retrieve the pointer to it to was set when the engine was initialized
		GraphicsEngine* GEPointer = (GraphicsEngine*)glfwGetWindowUserPointer(window);	
		
		/*if(key== GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);*/

		// Do any key translations here in the event that the keycode on this platform does not match our definition.

		// Then add the key to the input list
		GEPointer->QueueInputItem( InputItem( GE_INPUT_KEY, key, action, GEvec2( 0.0f, 0.0f ) ) );
	
	}

	static void mouse_position_callback( GLFWwindow* window, double xpos, double ypos )
	{
		// since this function is not a part of this class officially we need to 
		// retrieve the pointer to it to was set when the engine was initialized
		GraphicsEngine* GEPointer = (GraphicsEngine*)glfwGetWindowUserPointer(window);

		GEPointer->QueueInputItem( InputItem( GE_INPUT_MOUSEPOSITION, -1, GE_ACTION_NONE, GEvec2( (float)xpos, (float)ypos ) ) );
	}
	
	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		

		float aspect = (float)width/(float)height;
		glViewport(0,0,width,height);
		
	}

	static void mouse_button_callback( GLFWwindow* window, int button, int action, int mods )
	{
		GraphicsEngine* GEPointer = (GraphicsEngine*)glfwGetWindowUserPointer(window);

		GEPointer->QueueInputItem( InputItem( GE_INPUT_MOUSEBUTTON, button, action, GEvec2( 0.0f, 0.0f ) ) );
	}

	static void mouse_scroll_callback( GLFWwindow* window, double xoffset, double yoffset )
	{
		GraphicsEngine* GEPointer = (GraphicsEngine*)glfwGetWindowUserPointer(window);

		GEPointer->QueueInputItem( InputItem( GE_INPUT_MOUSESCROLL, -1, GE_ACTION_NONE, GEvec2( (float)xoffset, (float)yoffset ) ) );
	}


	//super bible static rain example below here.
	unsigned int seed;

	float random_float()
	{
		float res;
		unsigned int tmp;

		seed *= 16807;

		tmp = seed ^ (seed >> 4) ^ (seed << 15);

		*((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

		return (res - 1.0f);
	}

};

#endif // GRAPHICSENGINEOPENGL_H
