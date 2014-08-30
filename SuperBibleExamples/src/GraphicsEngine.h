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

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glext.h"
#include "vmath.h"  //vector math header from OpenGL Super Bible.  TODO Probably best to replace this at some point.

#include "ShaderManager.h"
#include "GEObject.h"
#include "GEMesh.h"
#include "GEShader.h"

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
	
	// std::map< std::string, GLuint > vaoMap;	// each mesh type will have its own vertex array object, the key will be the mesh class. 
	// std::map< std::string, GLuint > vbMap;		// one vb for each vao... for now unless it becomes unwieldly
	std::map< std::string, GEMesh > meshMap;	// Holds all the mesh information for the Graphics Engine.
	//std::map< std::string, GLuint > shaderMap;	// compiled shaders programs.  shadername/path, pointer to shader.
	std::map< std::string, GEShader > shaderMap;	// compiled shaders programs.  shadername/path, pointer to shader.
	
	ShaderManager shaderMan;	//for shader operations

	//some temp variables for the alien rain example
	GLuint          rain_buffer;

    float           droplet_x_offset[256];
    float           droplet_rot_speed[256];
    float           droplet_fall_speed[256];

public:

	

	//constructors
	GraphicsEngine();
	GraphicsEngine( const std::map< std::string, GEObject* >* gameEntities );  // passes a pointer to the game entities.
	~GraphicsEngine();

	static vmath::mat4 proj_matrix;	//for the global projection matrix

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
	bool isShaderBuffered( std::string shaderPath );
	bool BufferMesh( std::string meshPath, GEVertex* mesh, int numVerts );
	bool BufferShader( std::string shaderPath );
	
	
	
	//do these static functions need to be here?  They didn't seem to work if moved out of the header
	static void error_callback(int error, const char* description)
	{
		fputs(description, stderr);
	}

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if(key== GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}

	static void window_size_callback(GLFWwindow* window, int width, int height)
	{
		

		float aspect = (float)width/(float)height;
		proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
		glViewport(0,0,width,height);
		
		
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

#endif //GRAPHICSENGINE_H
