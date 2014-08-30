
/**
	ShaderManager.cpp
	Purpose: Handles all Graphic Shader Functions.

	@author Jeff Adams
*/
#include <string>

#include "ShaderManager.h"
#include "GEUtilities.h"

/**
	Default Constructor
*/
ShaderManager::ShaderManager()
{
	//Define the relative path to the shader directory. 
	materialPath = "./materials/";
	shaderPath = "./shaders/";
}

/**
	Load and compile a material from disk
	@ param matName - name of a material on disk in the materials folder.  Filename should be matName.mat.
	@ return - compiled program.
*/
GEShader ShaderManager::LoadMaterial( const std::string matName )
{
	GEShader returnShader;

	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint tessControlShader;
	GLuint tessEvalShader;
	GLuint geometryShader;
	GLuint program = 0;

	GLenum err;

	std::ifstream inFile;
	std::string fBuffer;

	std::string filename = materialPath + matName + ".mat";

	std::string vertexShaderPath;
	std::string tessControlShaderPath;
	std::string tessEvalShaderPath;
	std::string geometryShaderPath;
	std::string fragmentShaderPath;


	inFile.open ( filename, std::ifstream::in );

	if ( inFile.is_open() )
	{
		// go through each line of the file and determine what it is setting.
		while ( std::getline (inFile,fBuffer) )
		{
			// find where the '=' char is
			int equalPos = fBuffer.find( '=' );
			if ( equalPos != std::string::npos )
			{
				// split each line
				std::string mParameter = RTrim( fBuffer.substr( 0, equalPos ) );
				std::string mValue = LTrim( fBuffer.substr( equalPos + 1, std::string::npos ) );

				// evaluate each parameter
				if (mParameter == "vertex_shader")
				{
					if (!mValue.empty() )
						vertexShaderPath = mValue;
				}
				else if (mParameter == "tess_control_shader")
				{
					if (!mValue.empty() )
						tessControlShaderPath = mValue;
				}
				else if (mParameter == "tess_eval_shader")
				{
					if (!mValue.empty() )
						tessEvalShaderPath = mValue;
				}
				else if (mParameter == "geometry_shader")
				{
					if (!mValue.empty() )
						geometryShaderPath = mValue;
				}
				else if (mParameter == "fragment_shader")
				{
					if (!mValue.empty() )
						fragmentShaderPath = mValue;
				}
				// TODO: textures.

				printf( "Shader Param: '%s', Value: '%s'\n",mParameter.c_str(),mValue.c_str() );
			}
		}
		// close file... why am I even commenting this?
		inFile.close();

		// load the shaders
		program = glCreateProgram();

		// TODO remove repeating code.
		if ( !vertexShaderPath.empty() )
		{
			vertexShader = CompileShaderFromSource( vertexShaderPath.c_str(), GL_VERTEX_SHADER );
			glAttachShader(program, vertexShader);
			err = glGetError();
			if (err != GL_NO_ERROR )
			{
				printf("Attach Error: %d", err); 
			}
			glDeleteShader( vertexShader );
		}

		if ( !tessControlShaderPath.empty() )
		{
			tessControlShader = CompileShaderFromSource( tessControlShaderPath.c_str(),GL_TESS_CONTROL_SHADER );
			glAttachShader(program, tessControlShader);
			err = glGetError();
			if (err != GL_NO_ERROR )
			{
				printf("Attach Error: %d", err); 
			}
			glDeleteShader( tessControlShader );
		}

		if ( !tessEvalShaderPath.empty() )
		{
			tessEvalShader = CompileShaderFromSource( tessEvalShaderPath.c_str(), GL_TESS_EVALUATION_SHADER );
			glAttachShader(program, tessEvalShader);
			err = glGetError();
			if (err != GL_NO_ERROR )
			{
				printf("Attach Error: %d", err); 
			}
			glDeleteShader( tessEvalShader );
		}

		if ( !geometryShaderPath.empty() )
		{
			geometryShader = CompileShaderFromSource( geometryShaderPath.c_str(), GL_GEOMETRY_SHADER );
			glAttachShader(program, geometryShader);
			err = glGetError();
			if (err != GL_NO_ERROR )
			{
				printf("Attach Error: %d", err); 
			}
			glDeleteShader( geometryShader );
		}

		if ( !fragmentShaderPath.empty() )
		{
			fragmentShader = CompileShaderFromSource( fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER );
			glAttachShader(program, fragmentShader);
			err = glGetError();
			if (err != GL_NO_ERROR )
			{
				printf("Attach Error: %d", err); 
			}
			glDeleteShader( fragmentShader );
		}


		//fragmentShader = CompileShaderFromSource( fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER );
		glLinkProgram(program);
		err = glGetError();
		if (err != GL_NO_ERROR )
		{
			printf("Link Error: %d", err); 
		}
	}
	else
	{
		printf("Failed Opening Material: %s\n",filename.c_str());
	}

	returnShader.setProgram ( program );
	// set textures here.... if it has any.

	return returnShader;
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
