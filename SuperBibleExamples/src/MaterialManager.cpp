
/**
	ShaderManager.cpp
	Purpose: Handles all Graphic Shader Functions.

	@author Jeff Adams
*/
#include <string>
#include <stdio.h>

#include "MaterialManager.h"
#include "GEUtilities.h"


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
	GLuint vertexShader = 0;
	GLuint fragmentShader = 0;
	GLuint tessControlShader = 0;
	GLuint tessEvalShader = 0;
	GLuint geometryShader = 0;

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

	printf( "\nCreating Material: %s\n===============================================================\n", matName.c_str() );
	printf( "Loading Definition: %s...", filename.c_str() );

	inFile.open ( filename, std::ifstream::in );

	if ( inFile.is_open() )
	{
		printf( "Success\n" );

		// go through each line of the file and determine what it is setting.

		printf( "Reading Definition:\n");

		while ( std::getline (inFile,fBuffer) )
		{
			// find where the '=' char is
			int equalPos = fBuffer.find( '=' );
			if ( equalPos != std::string::npos )
			{
				// split each line
				std::string mParameter = LRTrim( fBuffer.substr( 0, equalPos ) );
				std::string mValue = LRTrim( fBuffer.substr( equalPos + 1, std::string::npos ) );

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

				if ( !mValue.empty() )
					printf( "\tShader Param: '%s', Value: '%s'\n",mParameter.c_str(),mValue.c_str() );
			}
		}
		// close file... why am I even commenting this?
		inFile.close();

		// load the shaders
		program = glCreateProgram();	// TODO: need to error check this?

		// TODO: Change the next five if statments into a loop?
		if ( !vertexShaderPath.empty() && success )  // if a vertex shader was specified and something else hasn't failed yet.
		{
			printf( "Vertex Shader: %s\n", vertexShaderPath.c_str() );
			vertexShader = CompileShaderFromSource( vertexShaderPath.c_str(), GL_VERTEX_SHADER );
			if ( vertexShader != 0 )
			{
				AttachShaderToProgram( vertexShader, program );  //TODO: how do we error check this?
				glDeleteShader( vertexShader );
			}
			else
				success = false;
		}

		if ( !tessControlShaderPath.empty() && success )
		{
			printf( "Tesselation Control Shader: %s\n", tessControlShaderPath.c_str() );
			tessControlShader = CompileShaderFromSource( tessControlShaderPath.c_str(), GL_TESS_CONTROL_SHADER );
			if( tessControlShader != 0 )
			{
				AttachShaderToProgram( tessControlShader, program );
				glDeleteShader( tessControlShader );
			}
			else
				success = false;
		}

		if ( !tessEvalShaderPath.empty() && success )
		{
			printf( "Tesselation Evaluation Shader: %s\n", tessEvalShaderPath.c_str() );
			tessEvalShader = CompileShaderFromSource( tessEvalShaderPath.c_str(), GL_TESS_EVALUATION_SHADER );
			if( tessEvalShader != 0)
			{
				AttachShaderToProgram( tessEvalShader, program );
				glDeleteShader( tessEvalShader );
			}
			else
				success = false;
		}

		if ( !geometryShaderPath.empty() && success )
		{
			printf( "Geomentry: %s\n", geometryShaderPath.c_str() );
			geometryShader = CompileShaderFromSource( geometryShaderPath.c_str(), GL_GEOMETRY_SHADER );
			if( geometryShader != 0 )
			{
				AttachShaderToProgram( geometryShader, program );
				glDeleteShader( geometryShader );
			}
			else
				success = false;
		}

		if ( !fragmentShaderPath.empty() && success )
		{
			printf( "Fragment Shader: %s\n", fragmentShaderPath.c_str() );
			fragmentShader = CompileShaderFromSource( fragmentShaderPath.c_str(), GL_FRAGMENT_SHADER );
			if( fragmentShader != 0)
			{
				AttachShaderToProgram( fragmentShader, program );
				glDeleteShader( fragmentShader );
			}
			else
				success = false;
		}

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

		if ( !success )
		{
			glDeleteProgram( program );
			program = 0;	
		}
		
		returnMaterial.setProgram( program );
			
		// set textures here.... if it has any.

	}
	else  // could not open file.
	{
		printf("Failed\n");
		returnMaterial.setProgram( 0 );		// set no program when failed.
	}

	if( returnMaterial.getProgram() != 0 )
	{
		printf( "Succeeded creating material.\n\n" );
	}
	else
		printf( "Failed Creating material.\n\n" );

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

/**
	Create a shader from an array of char strings

	@param source - Array of null terminated source strings.  Currently only makes use of first
	@param shaderType - Type of shader we are making
	@return - created shader
*/
GLuint MaterialManager::CreateShaderFromSource(const GLchar** source, GLenum shaderType )
{
	GLuint shader = 0;



	shader = glCreateShader(shaderType);

	if (shader != 0)
	{
		glShaderSource(shader, 1, source, NULL);
	}

	return shader;
}

GLuint MaterialManager::CompileShaderFromSource(const char* filename, GLenum shaderType )
{
	//vars
	GLuint shader = 0;
	std::string shaderSource;
	GLint return_code;

	shaderSource = LoadShaderSource(filename);

	if( shaderSource != "" )
	{

		const GLchar* shaderSourceArray[]={ shaderSource.c_str() };

		shader = CreateShaderFromSource( shaderSourceArray, shaderType );  // TODO error check this.
	
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
