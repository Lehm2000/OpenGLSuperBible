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
	
	std::string LoadShaderSource(const char* filename);
	GLuint CreateShaderFromSource(const GLchar** source, GLenum shaderType );
	GLuint CompileShaderFromSource(const char* filename, GLenum shaderType );

	/**
		@param shader
		@param program
		@param shaderName - solely for debug.
		@return void
	*/
	void AttachShaderToProgram( GLuint shader, GLuint program, std::string shaderName);
	
};

#endif /* MATERIALMANAGER_H */
