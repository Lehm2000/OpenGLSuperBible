#ifndef GRAPHICSENGINE_H
#define GRAPHICSENGINE_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glext.h"
#include "vmath.h"  //vector math header from OpenGL Super Bible.  TODO Probably best to replace this at some point.

#include "ShaderManager.h"

//temporary place to define vertex struct
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
private:
	GLFWwindow* window;
	GLuint rendering_program;
	GLuint vertex_array_object;
	
	

	ShaderManager shaderMan;	//for shader operations
public:

	//constructors
	GraphicsEngine();

	static vmath::mat4 proj_matrix;	//for the global projection matrix

	//functions... need to organize
	void Render(double currentTime);
	bool Init();
	void LoadExtensions(void);
	bool CleanUp();
	int CheckWindowClose();
	GLuint InitShaders(void);
	void InitBuffers(void);
	void InitTextures(void);
	void UpdateWindowSize(int x, int y, int width, int height);
	
	//Getters
	double getCurrentTime();
	
	
	
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


};

#endif //GRAPHICSENGINE_H
