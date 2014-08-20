
/**
	ShaderManager.cpp
	Purpose: Handles all Graphic Shader Functions.

	@author Jeff Adams
*/
#include <string>

#include "ShaderManager.h"

/**
	Default Constructor
*/
ShaderManager::ShaderManager()
{
	//Define the relative path to the shader directory. 
	shaderPath = "./shaders/";
}

/**
	Load a shader from disk

	@param filename - file on disk that will be loaded
	@param shaderSource - pointer to location that file data will be returned to
	@return void
*/
std::string ShaderManager::LoadShaderSource(const char* filename)
{
	 
	std::string sbuffer;	//create a buffer to hold data as it is read from file.
	std::string shader;		//shader to be returned.
	std::ifstream inFile;	//input file stream for reading file.
	GLchar* shaderSource;
	
	char fullFilename[1024];	//TODO: deal with magic number here.
	
	//combine filepath for shaders with supplied filename.
	strcpy(fullFilename,shaderPath.c_str());
	strcat(fullFilename,filename);
	
	inFile.open (fullFilename, std::ifstream::in);
	
	//TODO: Get some error handling up in here.
	//check if file is open.
	if (inFile.is_open())
	{
		//loop while data is being returned.
		while ( std::getline (inFile,sbuffer) )
		{
			shader += sbuffer +"\n";	//add buffer to the shader
		}
		//close file... why am I even commenting this.
		inFile.close();
	}
	else
	{
		printf("%s Failed Opening\n",filename);
	}

	return shader;
}

/**
	Create a shader from an array of char strings

	@param source - Array of null terminated source strings.  Currently only makes use of first
	@param shaderType - Type of shader we are making
	@return - created shader
*/
GLuint ShaderManager::CreateShaderFromSource(const GLchar** source, GLenum shaderType )
{
	GLuint shader;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, source, NULL);

	return shader;
}

/**
	Load, Create and then Compile a shader from a file.

	@param filename - file on disk that will be loaded
	@param shaderType - Type of shader we are making
	@return - compiled shader
*/
GLuint ShaderManager::CompileShaderFromSource(const char* filename, GLenum shaderType )
{
	//vars
	GLuint shader;
	std::string shaderSource;
	GLint return_code;



	shaderSource = LoadShaderSource(filename);

	const GLchar* shaderSourceArray[]={ shaderSource.c_str() };

	shader = CreateShaderFromSource(shaderSourceArray,shaderType );
	
	glCompileShader(shader);

	//check success
	glGetShaderiv(shader, GL_COMPILE_STATUS, &return_code);
	if (return_code == GL_TRUE)  //TODO: Better error handling.
	{
		printf("%s Compiled\n",filename);
	}
	else
	{
		//TODO: Better error handling.
		printf("%s Failed\n",filename);

		int logLength;
		GLchar log[2048];
		glGetShaderInfoLog(shader,2048,&logLength,log);
		printf(log);
	}

	return shader;
}
