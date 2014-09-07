

#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "TextureManager.h"
#include "ImageUtilities\ImageUtilities.h"
#include "PerspectiveCamera.h"
#include "ViewportInfo.h"


//Link static variables
vmath::mat4 GraphicsEngine::proj_matrix;

GraphicsEngine::GraphicsEngine()
{
	seed = 0x13371337;  //here just for the alien rain example

	//Initialize Graphics upon instantiation.  TODO Might want to make the program make an explicit call.  For now this works
	if(!Init())
	{
		printf("Error Initializing Graphics");
		exit(EXIT_FAILURE);
	}

	
}

GraphicsEngine::GraphicsEngine( const std::map< std::string, GEObject* >* gameEntities )
{
	seed = 0x13371337;  //here just for the alien rain example

	this->gameEntities = gameEntities;

	//Initialize Graphics upon instantiation.  TODO Might want to make the program make an explicit call.  For now this works
	if(!Init())
	{
		printf("Error Initializing Graphics");
		exit(EXIT_FAILURE);
	}
}

GraphicsEngine::~GraphicsEngine()
{
	// clean up all pointers here

	//voa

	//vb
}

// this version of render is for tutorial code.
void GraphicsEngine::Render(const double currentTime)
{
	// get the viewport info out of the game entities
	std::map< std::string, GEObject* >::const_iterator vpIt = (*gameEntities).find("SYS_Viewport_Options");
	ViewportInfo* viewportInfo = (ViewportInfo*)vpIt->second;

	// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
	glm::mat4 viewMatrix;
	std::map< std::string, GEObject* >::const_iterator camIt = (*gameEntities).find("gameCam");
	CameraObject* gameCam = (CameraObject*)camIt->second;
	if (gameCam->getClassName() == "PerspectiveCamera" )
	{
		viewMatrix = glm::perspective( ((PerspectiveCamera*)gameCam)->getFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f) * gameCam->GetViewMatrix();
	}

	const GLfloat bkColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat one = 1.0f;
	
	glClearBufferfv(GL_COLOR, 0, bkColor);
	glClearBufferfv(GL_DEPTH,0, &one);

	unsigned int renderCount = 0;

	// Iterate throught game entities here.
	for ( std::map< std::string, GEObject* >::const_iterator it = gameEntities->begin(); it != gameEntities->end(); it++ )
	{
		if ( it->second->isVisible() )
		{
			renderCount++;

			GLuint colorSubs[3];
			GEMaterial renderMaterial;
			
			renderMaterial = materialMap[ "SolidRGB_Subroutine" ];

			glUseProgram ( renderMaterial.getProgram() );
			
			GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "world_matrix" );
			GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "view_matrix" );

			colorSubs[0] = glGetProgramResourceIndex( renderMaterial.getProgram(), GL_FRAGMENT_SUBROUTINE, "colorSubRed" );
			colorSubs[1] = glGetProgramResourceIndex( renderMaterial.getProgram(), GL_FRAGMENT_SUBROUTINE, "colorSubGreen" );
			colorSubs[2] = glGetProgramResourceIndex( renderMaterial.getProgram(), GL_FRAGMENT_SUBROUTINE, "colorSubBlue" );

			glUniformSubroutinesuiv( GL_FRAGMENT_SHADER, 1, &colorSubs[ renderCount % 3 ] );

			GEMesh renderMesh = meshMap[ it->second->getMesh() ];

			glBindVertexArray( renderMesh.getVOA() );

			glm::mat4 worldMatrix = it->second->GetTransformMatrix();

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            
			glDrawArrays( renderMesh.getMeshType() , 0, renderMesh.getNumIndices() );
		}
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

// this version of render is for the actual game engine.
void GraphicsEngine::Render(const double currentTime, const std::map< std::string, GEObject* >* gameEntities_notused)
{
	// get the viewport info out of the game entities
	std::map< std::string, GEObject* >::const_iterator vpIt = (*gameEntities).find("SYS_Viewport_Options");
	ViewportInfo* viewportInfo = (ViewportInfo*)vpIt->second;

	// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
	glm::mat4 viewMatrix;
	std::map< std::string, GEObject* >::const_iterator camIt = (*gameEntities).find("gameCam");
	CameraObject* gameCam = (CameraObject*)camIt->second;
	if (gameCam->getClassName() == "PerspectiveCamera" )
	{
		viewMatrix = glm::perspective( ((PerspectiveCamera*)gameCam)->getFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f) * gameCam->GetViewMatrix();
	}

	const GLfloat bkColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat one = 1.0f;
	
	glClearBufferfv(GL_COLOR, 0, bkColor);
	glClearBufferfv(GL_DEPTH,0, &one);

	// get view matrix

	// Iterate throught game entities here.
	for ( std::map< std::string, GEObject* >::const_iterator it = gameEntities->begin(); it != gameEntities->end(); it++ )
	{
		if ( it->second->isVisible() )
		{
			GEMaterial renderMaterial;
			std::string renderMaterialName = it->second->getMaterial();

			std::map< std::string, GEMaterial>::iterator mapIt = materialMap.find( renderMaterialName );

			if ( mapIt != materialMap.end() )
			{
				renderMaterial = materialMap[ renderMaterialName ];
			}
			else
			{
				// if the material specified for this object is not found use the default
				renderMaterial = materialMap[ "default" ];
			}

			glUseProgram ( renderMaterial.getProgram() );
			
			GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "world_matrix" );
			GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "view_matrix" );

			GEMesh renderMesh = meshMap[ it->second->getMesh() ];

			glBindVertexArray( renderMesh.getVOA() );

			glm::mat4 worldMatrix = it->second->GetTransformMatrix();

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            
			glDrawArrays( renderMesh.getMeshType() , 0, renderMesh.getNumIndices() );
		}
	}

	glfwSwapBuffers(window);
	glfwPollEvents();
}

bool GraphicsEngine::Init()
{
	glfwSetErrorCallback(error_callback);

	//initial OpenGL (glfw)
	if(!glfwInit())
		//return false;
		exit(EXIT_FAILURE);

	std::map< std::string, GEObject* >::const_iterator vpIt = (*gameEntities).find("SYS_Viewport_Options");
	ViewportInfo* viewportInfo = (ViewportInfo*)vpIt->second;

	//vars for window dimensions... temporary
	//int winWidth = 640;
	//int winHeight = 480;

	//	Added so that OSX would use OpenGL 3.2 instead of the default 2.1
	/**
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	*/

	// set window hints
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// create the window
	window = glfwCreateWindow( viewportInfo->getViewportWidth(), viewportInfo->getViewportHeight(), "OpenGL Super Bible", NULL,NULL);
	if(!window)
	{
		glfwTerminate();
		return false;
		//exit(EXIT_FAILURE);
	}



	//create initial projection matrix TODO: remove hardcoded numbers.
	proj_matrix = vmath::perspective(50.0f, (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f);
	
	glfwMakeContextCurrent(window);

	//initialize openGL extensions.  Must be done after making context current. Otherwise will error
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  //fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		printf("Error: %s\n", glewGetErrorString(err));
	}

	// print some OpenGL info
	printf( "GL_VENDOR: %s\n",glGetString(GL_VENDOR));
	printf( "GL_RENDERER: %s\n",glGetString(GL_RENDERER));
	printf( "GL_VERSION: %s\n",glGetString(GL_VERSION));
	printf( "GL_SHADING_LANGUAGE_VERSION: %s\n\n",glGetString(GL_SHADING_LANGUAGE_VERSION));

	// specify some callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowSizeCallback(window, window_size_callback);
	
	//not sure this is the best place for these, but will work for now
	InitShaders();
	InitBuffers();
	InitTextures();
	
	//Set some options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	
	return true;
}

bool GraphicsEngine::CleanUp()
{

	glDeleteVertexArrays(1, &vertex_array_object);
	glDeleteProgram(rendering_program);
	glDeleteVertexArrays(1, &vertex_array_object);

	glfwDestroyWindow(window);
	glfwTerminate();

	return true;  //eventually change to success or fail.
}


int GraphicsEngine::CheckWindowClose()
{
	return glfwWindowShouldClose(window);
}

double GraphicsEngine::getCurrentTime() const
{
	return glfwGetTime();
}

bool GraphicsEngine::InitShaders(void)
{
	// Temp tutorial code.

	BufferMaterial( "alien_rain" );
	BufferMaterial( "SolidRGB_Subroutine" );

	/*GLuint vertex_shader;
	GLuint fragment_shader;
	GLuint tess_control_shader;
	GLuint tess_eval_shader;
	GLuint geometry_shader;
	GLuint program;

	std::string tempString;
	GLint return_code;

	//Create vertex shader
	vertex_shader = materialMan.CompileShaderFromSource("alien_rain_vert.glsl", GL_VERTEX_SHADER );
	
	//Create tess control shader
	tess_control_shader = materialMan.CompileShaderFromSource("tess_control_shader_source.txt", GL_TESS_CONTROL_SHADER);

	//Create tess eval shader
	tess_eval_shader = materialMan.CompileShaderFromSource("tess_eval_shader_source.txt", GL_TESS_EVALUATION_SHADER);

	//Create geometry shader
	geometry_shader = materialMan.CompileShaderFromSource("geometry_shader_source.txt", GL_GEOMETRY_SHADER);

	//Create fragment shader
	fragment_shader = materialMan.CompileShaderFromSource("alien_rain_frag.glsl", GL_FRAGMENT_SHADER);
	

	
	//Create program, attach shaders to it, and link it... need to put this code inside the shader manager
	//TODO: Error handling.
	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	//glAttachShader(program, tess_control_shader);
	//glAttachShader(program, tess_eval_shader);
	//glAttachShader(program, geometry_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	rendering_program = program;
	
	

	// Delete the shaders as the program has them now
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	*/

	return true;
}
/**
	Place to experiment with Buffers
*/
void GraphicsEngine::InitBuffers(void)
{
	GLuint buffer[2];

	// playin with vertex array objects
	
	glGenVertexArrays(1, &vertex_array_object);
	glBindVertexArray(vertex_array_object);

	// This is the data that we will place into the buffer object

	glGenBuffers(1, &rain_buffer);
    glBindBuffer(GL_UNIFORM_BUFFER, rain_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 256 * sizeof(vmath::vec4), NULL, GL_DYNAMIC_DRAW);

    for (int i = 0; i < 256; i++)
    {
        droplet_x_offset[i] = random_float() * 2.0f - 1.0f;
        droplet_rot_speed[i] = (random_float() + 0.5f) * ((i & 1) ? -3.0f : 3.0f);
        droplet_fall_speed[i] = random_float() + 0.2f;
    }

}

void GraphicsEngine::InitTextures(void)
{
	
	//	testing loading textures here

	TextureManager texMan;
	GLenum glError = 0;
	unsigned char returnType = 0;

	IUImage<unsigned char> testImage1 = texMan.LoadTexture( "test2.bmp", GE_TEXTYPE_BMP, returnType );
	IUImage<unsigned char> testImage2 = texMan.LoadTexture( "test2r.bmp", GE_TEXTYPE_BMP, returnType );
	
	unsigned char* data[2];
	data[0] = (unsigned char*)malloc( testImage1.getDataSize() );
	data[1] = (unsigned char*)malloc( testImage2.getDataSize() );
	
	testImage1.getData(data[0]);
	testImage2.getData(data[1]);

	GLuint texture;

	GLenum internalFormat;
	GLenum imageFormat;

	if (testImage1.getNumChannels() == 3)
	{
		internalFormat = GL_RGB32F;
		imageFormat = GL_RGB;
	}
	else
	{
		internalFormat = GL_RGBA32F;
		imageFormat = GL_RGBA;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY ,texture);

	glError = glGetError();
	if (glError != GL_NO_ERROR)
	{
		printf( "Error binding texture: %d\n", glError );
	}

	//glTexStorage3D(GL_TEXTURE_2D_ARRAY, 4, GL_RGB8, 256, 256, 2);
	glTexStorage3D( GL_TEXTURE_2D_ARRAY, 5, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, 256, 256, 2 );

	glError = glGetError();
	if (glError != GL_NO_ERROR)
	{
		printf( "Error generating texture buffer: %d\n", glError );
	}

	for (int i = 0; i < 2; i++)
	{
		glTexSubImage3D( GL_TEXTURE_2D_ARRAY,0,0,0,i,256,256,1, GL_RGB, GL_UNSIGNED_BYTE, data[i] );
		
		glError = glGetError();
		if (glError != GL_NO_ERROR)
		{
			printf( "Error copying texture to buffer: %d\n", glError );
		}
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	create the mipmaps.
	glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

	//	now we generate the sampler... optional (a default sampler will be assigned otherwise)
	//GLuint sampler;
	//glGenSamplers(1, &sampler);

	//	set some sampler options
	//glSamplerParameteri(sampler,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	//glSamplerParameteri(sampler,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	now bind it to a texture unit
	//glBindSampler(0, sampler);	//bind it to texture unit 0... the only one we are using currently.
	
	//	clean up
	delete [] data[0];
	delete [] data[1];

}

void GraphicsEngine::UpdateWindowSize(int x, int y, int width, int height)
{
	float aspect = (float)width/(float)height;
	proj_matrix = vmath::perspective(50.0f, aspect, 0.1f, 1000.0f);
	glViewport(x,y,width,height);
}

bool GraphicsEngine::isMeshBuffered( std::string meshPath)
{
	// check if mesh already loaded
	bool buffered = true;

	// if find == the end... its not in the map
	if ( meshMap.find( meshPath ) == meshMap.end() )
	{
		buffered = false;
	}

	return buffered;
}

bool GraphicsEngine::isMaterialBuffered( std::string materialPath )
{
	// check if mesh already loaded
	bool buffered = true;

	// if find == the end... its not in the map
	if ( materialMap.find( materialPath ) == materialMap.end() )
	{
		buffered = false;
	}

	return buffered;
}

bool GraphicsEngine::BufferMesh( std::string meshPath, GEVertex* mesh, int numVerts )
{
	// Create GLuint to hold new voa.  Each Mesh gets its own voa.
	GLuint newVoa;
	
	// Create a new buffer
	GLuint newBuffer;
	
	glGenVertexArrays( 1, &newVoa );
	glBindVertexArray( newVoa );

	glGenBuffers( 1, &newBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, newBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GEVertex ) * numVerts, mesh, GL_STATIC_DRAW );

	// Position
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, x ));
	glEnableVertexAttribArray(0);
	
	// Color
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, r ));
	glEnableVertexAttribArray(1);

	// Normals
	// Texture Coords.

	// Next put mesh into the map

	GEMesh newMesh( GL_TRIANGLES, numVerts, newVoa, newBuffer );  // TODO: where does GL_TRIAnGLES come from?
	meshMap[meshPath] = newMesh;

	// Unbind voa??  Don't for now.

	return true;
}

bool GraphicsEngine::BufferMaterial( std::string materialPath )
{
	bool success = false; 

	printf( "Buffering Material: %s\n====================================\n", materialPath.c_str() );

	GEMaterial newMaterial = materialMan.LoadMaterial( materialPath );

	// Verify the material loaded

	if ( newMaterial.getProgram() != 0 )
	{
		// Add it to the shader map
	 
		materialMap.insert( std::pair< std::string, GEMaterial >( materialPath, newMaterial ) );

		success = true;

		printf ( "Buffering Complete\n");
	}
	else
	{
		printf ( "Buffering Failed\n");
	}


	return success;
}

