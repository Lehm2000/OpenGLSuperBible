#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glext.h"
#include "GEMaterial.h"

class MaterialManager
{
private:
	// Members

	std::string materialPath;
	std::string shaderPath;

public:

	//constructors
	MaterialManager();

	//Functions
	GEMaterial LoadMaterial( const std::string matName );
	
	std::string LoadShaderSource( const char* filename);
	GLuint CreateShaderFromSource( const GLchar** source, GLenum shaderType );
	
	/**
		
	*/
	GLuint CompileShaderFromSource(  const GLchar** source, GLenum shaderType );

	/**
		Load, Create and then Compile a shader from a file.

		@param filename - file on disk that will be loaded
		@param shaderType - Type of shader we are making
		@return - compiled shader ( 0 if failed )
	*/
	GLuint CompileShaderFromFile(const char* filename, GLenum shaderType );

	/**
		@param shader - shader that will be attached to the program
		@param program - program the shader is attached to.
		@return void
	*/
	void AttachShaderToProgram( GLuint shader, GLuint program );
	
};

#endif /* MATERIALMANAGER_H */
