#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glext.h"
#include "GEShader.h"

class ShaderManager
{
private:
	// Members

	std::string materialPath;
	std::string shaderPath;

public:

	//constructors
	ShaderManager();

	//Functions
	GEShader LoadMaterial( const std::string matName );
	
	std::string LoadShaderSource(const char* filename);
	GLuint CreateShaderFromSource(const GLchar** source, GLenum shaderType );
	GLuint CompileShaderFromSource(const char* filename, GLenum shaderType );
	
};

#endif /* SHADERMANAGER_H */
