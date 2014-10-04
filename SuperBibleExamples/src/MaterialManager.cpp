
/**
	ShaderManager.cpp
	Purpose: Handles all Graphic Shader Functions.

	@author Jeff Adams
*/
#include <string>
#include <stdio.h>

#include "MaterialManager.h"
#include "GEUtilities.h"
#include "GEConstants.h"

/**
	Default Constructor
*/
MaterialManager::MaterialManager()
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
GEMaterial MaterialManager::LoadMaterial( const std::string matName )
{
	GEMaterial returnMaterial;
	bool success = true;

	// Initial shader refs to 0 for error checking.
	GLuint shader = 0;

	GLuint program = 0;

	GLenum err;

	std::ifstream inFile;
	std::string fBuffer;

	std::string filename = materialPath + matName + ".mat";

	std::string resourcePath;
	

	GLenum shaderType;
	std::string resourceType = "";

	std::vector< std::string> textureList;


	// create the program object
	program = glCreateProgram();	// TODO: need to error check this?

	printf( "\nCreating Material: %s\n===============================================================\n", matName.c_str() );
	printf( "Loading Definition: %s...", filename.c_str() );

	inFile.open ( filename, std::ifstream::in );

	if ( inFile.is_open() )
	{
		printf( "Success\n" );

		// go through each line of the file and determine what it is setting.

		printf( "Reading Definition:\n");

		while ( std::getline (inFile,fBuffer) && success )
		{
			// find where the '=' char is
			int equalPos = fBuffer.find( '=' );
			if ( equalPos != std::string::npos )
			{
				// split each line
				std::string mParameter = LRTrim( fBuffer.substr( 0, equalPos ) );
				std::string mValue = LRTrim( fBuffer.substr( equalPos + 1, std::string::npos ) );

				resourceType = "";

				std::string texturePaths[ GE_MAX_TEXTURES ];

				if ( !mValue.empty() && success)
				{
					resourcePath = mValue;

					if ( mParameter == "vertex_shader" )
					{
						shaderType = GL_VERTEX_SHADER;
						resourceType = "shader";
					}
					else if (mParameter == "tess_control_shader")
					{
						shaderType = GL_TESS_CONTROL_SHADER;
						resourceType = "shader";
					}
					else if (mParameter == "tess_eval_shader")
					{
						shaderType = GL_TESS_EVALUATION_SHADER;
						resourceType = "shader";
					}
					else if (mParameter == "geometry_shader")
					{
						shaderType = GL_GEOMETRY_SHADER;
						resourceType = "shader";
					}
					else if (mParameter == "fragment_shader")
					{
						shaderType = GL_FRAGMENT_SHADER;
						resourceType = "shader";
					}
					else if ( mParameter.find ("texture") != std::string::npos )
					{
						// we've got ourselves a texture.

						// They should be in the right order so I'm going to ignore the number for now.
						resourceType = "texture";
						
					}

					// load the resource
					if ( !resourcePath.empty() && resourceType == "shader" ) 
					{
						// load a shader
						printf( "%s: %s\n", mParameter.c_str(), mValue.c_str() );
						shader = CompileShaderFromFile( resourcePath.c_str(), shaderType );
						if ( shader != 0 )
						{
							AttachShaderToProgram( shader, program );  //TODO: how do we error check this?
							glDeleteShader( shader );
						}
						else
							success = false;
					}
					else if( !resourcePath.empty() && resourceType == "texture" )
					{
						// add texture to the texture list
						printf( "Adding Texture Ref: %s\n", resourcePath.c_str() );
						textureList.push_back( resourcePath );

					}
				}

			}
		}
		
		// close file... why am I even commenting this?
		inFile.close();

		if ( success )
		{
			GLint return_code;

			printf( "Linking..." );

			glLinkProgram( program );

			//check success
			glGetProgramiv(program, GL_LINK_STATUS, &return_code);
			if (return_code == GL_TRUE)  
			{
				printf("Success\n");
			}
			else
			{
				printf("Failed\n");

				GLint logLength;

				glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logLength );

				GLchar* log = new GLchar[logLength + 1];
				glGetProgramInfoLog( program, logLength, NULL, log );

				printf(log);

				success = false;
			}
		}
		else
		{
			glDeleteProgram( program );
			program = 0;	
		}
		
		

	}
	else  // could not open file.
	{
		success = false;
		printf("Failed\n");
		
	}

	if (success)
	{
		returnMaterial.setProgram( program );
		returnMaterial.setTextures( textureList );
		printf( "Succeeded creating material.\n\n" );
	}
	else
	{
		glDeleteProgram( program );
		program = 0;
		returnMaterial.setProgram( program );		// set no program when failed.
		printf( "Failed Creating material.\n\n" );
	}

	return returnMaterial;
}

/**
	Load a shader from disk

	@param filename - file on disk that will be loaded
	@param shaderSource - pointer to location that file data will be returned to
	@return void
*/
std::string MaterialManager::LoadShaderSource(const char* filename)
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
	
	printf( "\tLoading Source..." );

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

		printf ( "Success\n" );
	}
	else
	{
		printf("Failed\n",filename);
		shader = "";
	}

	return shader;
}



GLuint MaterialManager::CreateShaderFromSource( const GLchar** source, GLenum shaderType )
{
	GLuint shader = 0;



	shader = glCreateShader(shaderType);

	if (shader != 0)
	{
		glShaderSource(shader, 1, source, NULL);
	}

	return shader;
}

GLuint MaterialManager::CompileShaderFromSource(  const GLchar** source, GLenum shaderType )
{
	GLuint shader = 0;
	GLint return_code;

	shader = CreateShaderFromSource( source, shaderType );  // TODO error check this.
	
	printf ( "\tCompiling..." );

	glCompileShader(shader);

	//check success
	glGetShaderiv(shader, GL_COMPILE_STATUS, &return_code);
	if (return_code == GL_TRUE)  
	{
		printf("Success\n");
	}
	else
	{
		printf("Failed\n");

		GLint logLength;

		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logLength );

		GLchar* log = new GLchar[logLength + 1];
		glGetShaderInfoLog( shader, logLength, NULL, log );

		printf(log);

		//clean up.
		glDeleteShader( shader );
		shader = 0;
	}

	return shader;
}

GLuint MaterialManager::CompileShaderFromFile( const char* filename, GLenum shaderType )
{
	//vars
	GLuint shader = 0;
	std::string shaderSource;
	

	shaderSource = LoadShaderSource(filename);

	if( shaderSource != "" )
	{

		const GLchar* shaderSourceArray[]={ shaderSource.c_str() };

		shader = CompileShaderFromSource( shaderSourceArray, shaderType );
	}
	

	return shader;
}

void MaterialManager::AttachShaderToProgram( GLuint shader, GLuint program )
{
	GLenum err;

	printf( "\tAttaching Shader...");

	glAttachShader(program, shader);
	err = glGetError();
	if (err != GL_NO_ERROR )
	{
		printf( "Failed: %d\n", err ); 
	}
	else
		printf( "Success\n");
	
}
