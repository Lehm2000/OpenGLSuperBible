#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

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
#include "vmath.h"  //vector math header from OpenGL Super Bible.  TODO Probably best to replace this at some point.

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
private:
	GLFWwindow* window;
	GLuint rendering_program;
	GLuint vertex_array_object;

	const std::map< std::string, GEObject* >* gameEntities;  // this is a pointer to the gameEntities... passed from the game engine when it was created.
	
	std::map< std::string, GEMesh > meshMap;	// Holds all the mesh information for the Graphics Engine.
	std::map< std::string, GEMaterial > materialMap;	// Holds all materials for the game.  Materials hold the shader plus references to the textures used.
	std::map< std::string, GLuint > textureMap;			// Holds textures for the game.
	std::queue< InputItem > InputList;					// Holds inputs that have been recorded.

	MaterialManager materialMan;	//for material operations

	//some temp variables for the alien rain example
	GLuint          rain_buffer;

    float           droplet_x_offset[256];
    float           droplet_rot_speed[256];
    float           droplet_fall_speed[256];

public:

	

	// structors
	GraphicsEngine();
	GraphicsEngine( const std::map< std::string, GEObject* >* gameEntities );  // passes a pointer to the game entities.
	~GraphicsEngine();

	// setters

	// getters
	std::queue< InputItem >* getInputList();

	static vmath::mat4 proj_matrix;	//for the global projection matrix.. TODO eliminate this.

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
	void Render(const double currentTime);

	/**
		Renders the scene. 

		@param currentTime - the time since the program started.
		@param gameEntities - pointer to the game Engine entities.
	*/
	void Render(const double currentTime, const std::map< std::string, GEObject* >* gameEntities);

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

	/**
		BufferMesh()
		Takes mesh and stores it in the map of mesh objects
		@param meshPath - unique mesh name/path.
		@param mesh - pointer to vertex data
		@param numVerts - number of vertices in the mesh.
		@param vertIndices - pointer to vertex indices
		@param numIndices
	*/
	bool BufferMesh( std::string meshPath, GEVertex* mesh, unsigned int numVerts, GLushort* vertIndices, unsigned int numIndices );
	bool BufferMaterial( std::string materialPath );
	
	void QueueInputItem( InputItem input );
	
	
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
		proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
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

#endif // GRAPHICSENGINE_H
