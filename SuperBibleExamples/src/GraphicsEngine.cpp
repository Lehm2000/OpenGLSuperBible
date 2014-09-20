

#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "TextureManager.h"
#include "ImageUtilities\ImageUtilities.h"
#include "CameraPerspective.h"
#include "InfoViewport.h"
#include "InfoGameVars.h"
#include "GEInputState.h"


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

// setters

// getters
std::queue< InputItem >* GraphicsEngine::getInputList()
{
	return &InputList;
}

// this version of render is for tutorial code.
void GraphicsEngine::Render(const double currentTime)
{
	// get the viewport info out of the game entities
	std::map< std::string, GEObject* >::const_iterator vpIt = gameEntities->find("SYS_Viewport_Options");
	InfoViewport* viewportInfo = (InfoViewport*)vpIt->second;

	// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
	glm::mat4 viewMatrix;
	std::map< std::string, GEObject* >::const_iterator camIt = gameEntities->find("gameCam");
	CameraObject* gameCam = (CameraObject*)camIt->second;
	if (gameCam->getClassName() == "CameraPerspective" )
	{
		viewMatrix = glm::perspective( ((CameraPerspective*)gameCam)->getFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 4.0f) * gameCam->GetViewMatrix();
	}

	const GLfloat bkColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
	const GLfloat one = 1.0f;
	
	glClearBufferfv(GL_COLOR, 0, bkColor);
	glClearBufferfv(GL_DEPTH,0, &one);

	std::map< std::string, GEMesh>::const_iterator meshIt = meshMap.find("beziersphere");

	GEMesh renderMesh = meshIt->second;

	glBindVertexArray( renderMesh.getVertexArrayObject() );
	
	GEMaterial renderMaterial;
			
	renderMaterial = materialMap.find("tessellation_testBezier")->second;

	glUseProgram ( renderMaterial.getProgram() );
			
	GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "world_matrix" );
	GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "view_matrix" );
	GLint tessLevelLocation = glGetUniformLocation( renderMaterial.getProgram(), "tessLevel" );
	GLint displaceTextureLoc = glGetUniformLocation( renderMaterial.getProgram(), "displaceTexture");

	//bind the displace texture
	//glBindTexture( GL_TEXTURE_2D, textureMap.find("DisplaceTest")->second );

	//glm::mat4 worldMatrix = it->second->GetTransformMatrix();
	glm::mat4 worldMatrix = glm::rotate( glm::mat4(),(float)currentTime / 4.0f,glm::vec3(0.0f, 1.0f, 0.0f ) )* glm::scale( glm::mat4(), glm::vec3(0.5f, 0.5f, 0.5f));

	glUniformMatrix4fv( worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0] );
	glUniformMatrix4fv( viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0] );
	//glUniform1f( tessLevelLocation, (sin((float)currentTime / 3.0) +1.0f)*10.0);
	glUniform1f( tessLevelLocation, (sin((float)currentTime / 2.0) * 5.0f ) + 6.0 );
	//glUniform1f( tessLevelLocation, 5.0);
    glUniform1i( displaceTextureLoc, 0 );
	glPatchParameteri( GL_PATCH_VERTICES, 4);
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//glDrawArrays(GL_PATCHES, 0 , 4);
	glEnable( GL_PRIMITIVE_RESTART );
	glPrimitiveRestartIndex( 0xFFFF );
	glDrawElements( GL_PATCHES,renderMesh.getNumIndices(),GL_UNSIGNED_SHORT, 0 );
	glDisable( GL_PRIMITIVE_RESTART );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	RenderFPS( currentTime );

	glfwSwapBuffers(window);
	glfwPollEvents();
}

// this version of render is for the actual game engine.
void GraphicsEngine::Render(const double currentTime, const std::map< std::string, GEObject* >* gameEntities_notused)
{
	// get the viewport info out of the game entities
	std::map< std::string, GEObject* >::const_iterator vpIt = gameEntities->find("SYS_Viewport_Options");
	InfoViewport* viewportInfo = (InfoViewport*)vpIt->second;

	// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
	glm::mat4 viewMatrix;
	std::map< std::string, GEObject* >::const_iterator camIt = gameEntities->find("gameCam");
	CameraObject* gameCam = (CameraObject*)camIt->second;
	if (gameCam->getClassName() == "CameraPerspective" )
	{
		viewMatrix = glm::perspective( ((CameraPerspective*)gameCam)->getFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f) * gameCam->GetViewMatrix();
	}

	const GLfloat bkColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	const GLfloat one = 1.0f;
	
	glClearBufferfv(GL_COLOR, 0, bkColor);
	glClearBufferfv(GL_DEPTH,0, &one);

	// Iterate throught game entities here.
	for (int i = 0; i <1; i++)
	{
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

			// DO NOT ACCESS MAP THIS WAY!!!!!!!.  CHANGE ASAP
			GEMesh renderMesh = meshMap[ it->second->getMesh() ];

			glBindVertexArray( renderMesh.getVertexArrayObject() );
			
			glm::mat4 worldMatrix = it->second->GetTransformMatrix();

			glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
			glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            
			//glDrawArrays( renderMesh.getMeshType() , 0, renderMesh.getNumIndices() );
			//glDrawElements( renderMesh.getMeshType(), renderMesh.getNumIndices(), GL_UNSIGNED_SHORT, 0);
			glEnable( GL_PRIMITIVE_RESTART );
			glPrimitiveRestartIndex( 0xFFFF );
			for (int j = 0; j<1; j++ )  // just to test performance
			{
				//glDrawElements( renderMesh.getMeshType(), renderMesh.getNumIndices(), GL_UNSIGNED_SHORT, 0);
				glBindBuffer( GL_DRAW_INDIRECT_BUFFER, renderMesh.getIndirectBuffer() );
				glDrawElementsIndirect( renderMesh.getMeshType(), GL_UNSIGNED_SHORT, 0);
				//glDrawElementsInstanced(  renderMesh.getMeshType(), renderMesh.getNumIndices(), GL_UNSIGNED_SHORT, 0, 10000);
			}
			glDisable( GL_PRIMITIVE_RESTART );
			/*for (int i = 0; i<6; i++)
			{
				glDrawElements( GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (void*)(4*i*sizeof(GLushort) ) );
			};*/
		}
	}
	}


	RenderFPS( currentTime );

	

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GraphicsEngine::RenderFPS(const double currentTime)
{
	// Testing text output

	// get the viewport info out of the game entities
	std::map< std::string, GEObject* >::const_iterator vpIt = gameEntities->find("SYS_Viewport_Options");
	InfoViewport* viewportInfo = (InfoViewport*)vpIt->second;
	
	//std::map< std::string, GEObject* >::const_iterator fontIt = gameEntities->find("SYS_FONT");
	GEMesh fontMesh = meshMap[ "SYS_FONT" ];
	
	glBindVertexArray( fontMesh.getVertexArrayObject() );

	GEMaterial fontMaterial = materialMap[ "SystemFont01" ];
	glUseProgram( fontMaterial.getProgram() );
	glActiveTexture( GL_TEXTURE0 );

	GLint fontTextureLoc = glGetUniformLocation( fontMaterial.getProgram(), "fontTexture");
	GLint screenMatrixLocation = glGetUniformLocation( fontMaterial.getProgram(), "screenMatrix" );
	GLuint charSizeLoc = glGetUniformLocation( fontMaterial.getProgram(), "charSize" );
	GLuint startPosLoc = glGetUniformLocation( fontMaterial.getProgram(), "startPos" );
	
	glUniform1f( charSizeLoc, 0.015f );
	glUniform2f( startPosLoc, 0.0075, 0.0075 );

	glm::mat4 screenMatrix = glm::ortho( 0.0f, 1.0f, (float)viewportInfo->getViewportHeight()/(float)viewportInfo->getViewportWidth(), 0.0f );

	glUniformMatrix4fv(screenMatrixLocation, 1, GL_FALSE, &screenMatrix[0][0]);
	
	glUniform1i(fontTextureLoc, 0);

	glBindTexture( GL_TEXTURE_2D, textureMap.find("SYS_Font01")->second );
	
	vpIt = gameEntities->find("SYS_Game_Vars");
	InfoGameVars* gameVars = static_cast<InfoGameVars*>(vpIt->second);

	std::string testString1 = "FPS: ";
	std::string testString2 = std::to_string( 1.0 / gameVars->getDeltaFrameTime() );
	testString1.append( testString2 );
	
	glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
	glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar)*testString1.length(), testString1.c_str());
	glVertexAttribDivisor(0, 1);
	glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, testString1.length() );
	glVertexAttribDivisor(0, 0);

	// temp render the pressed inputs.------------------------------------------

	// get pointer to the input state
	std::map< std::string, GEObject* >::const_iterator isIt = gameEntities->find("SYS_Input_State");
	GEInputState* inputState = (GEInputState*)isIt->second;

	std::string inputString = "";
	
	for ( unsigned int i = 0; i<INPUTSTATE_MAX_KEY_BUTTONS; i++ )
	{
		if (inputState->getKeyboardKey( i ) )  // if key pressed
		{
			inputString.append( inputState->KeyToString( i ) );  // get the string representation of it
		}
	}

	
	glUniform2f( startPosLoc, 0.0075, 0.0275 );
	glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
	glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar) * inputString.length(), inputString.c_str());
	glVertexAttribDivisor(0, 1);
	glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, inputString.length() );
	glVertexAttribDivisor(0, 0);
}

bool GraphicsEngine::Init()
{
	glfwSetErrorCallback(error_callback);

	// initial OpenGL (glfw)
	if( !glfwInit() )
		exit( EXIT_FAILURE );

	std::map< std::string, GEObject* >::const_iterator vpIt = gameEntities->find("SYS_Viewport_Options");
	InfoViewport* viewportInfo = (InfoViewport*)vpIt->second;


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

	// set the window pointer to this graphics engine so the callback functions have access to it.
	glfwSetWindowUserPointer( window, this ); 	
	
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
	BufferMaterial( "SystemFont01" );

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


	// Create the font buffers
	
	// Create GLuint to hold new voa.  Each Mesh gets its own voa.
	GLuint fontVOA;
	
	// Create a new buffers
	GLuint fontVBO;

	// Create the Vertex
	
	glGenVertexArrays( 1, &fontVOA );
	glBindVertexArray( fontVOA );

	glGenBuffers( 1, &fontVBO );
	glBindBuffer( GL_ARRAY_BUFFER, fontVBO );

	//GLuint testarray[] ={ 2,10};
	//glBufferData( GL_ARRAY_BUFFER, sizeof( GLchar ) * 2, testarray, GL_DYNAMIC_DRAW );
	//glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVB() );
	//glBufferSubData( GL_ARRAY_BUFFER,0,sizeof(GLuint) *2,&testarray );

	// create a buffer 80 characters long.
	//GLuint testarray[] ={ 'a','b','c'};
	glBufferData( GL_ARRAY_BUFFER, sizeof( GLubyte ) * 80, 0, GL_DYNAMIC_DRAW );
	//glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(testarray), &testarray);

	// Character Code
	glVertexAttribIPointer( 0, 1, GL_UNSIGNED_BYTE, 0, 0);
	glEnableVertexAttribArray(0);

	GEMesh newMesh( GL_TRIANGLES, 0, 0, fontVOA, fontVBO, 0,0);  // the font mesh doesn't really fit the GEMesh type... new type?
	meshMap["SYS_FONT"] = newMesh;
	
	

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


	// Load the font Texture
	IUImage<unsigned char> fontTex = texMan.LoadTexture( "font01.bmp", GE_TEXTYPE_BMP, returnType );
	
	unsigned char* fontTexData;
	fontTexData = (unsigned char*)malloc( fontTex.getDataSize() );
	fontTex.getData( fontTexData );

	GLuint fontTexture;
	glGenTextures(1, &fontTexture);
	glBindTexture(GL_TEXTURE_2D, fontTexture);

	glTexStorage2D( GL_TEXTURE_2D, 5, GL_RGB8, fontTex.getWidth(), fontTex.getHeight() );
	glTexSubImage2D( GL_TEXTURE_2D,0,0,0, fontTex.getWidth(), fontTex.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, fontTexData );

	glGenerateMipmap( GL_TEXTURE_2D );

	textureMap.insert( std::pair< std::string, GLuint >( "SYS_Font01", fontTexture ) );

	//	now we generate the sampler... optional (a default sampler will be assigned otherwise)
	GLuint sampler;
	glGenSamplers(1, &sampler);

	//	set some sampler options
	glSamplerParameteri(sampler,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(sampler,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	now bind it to a texture unit
	glBindSampler(0, sampler);	//bind it to texture unit 0... the only one we are using currently.

	
	
	// Load the displace Texture
	IUImage<unsigned char> displaceTex = texMan.LoadTexture( "displace_test.bmp", GE_TEXTYPE_BMP, returnType );
	
	unsigned char* displaceTexData;
	displaceTexData = (unsigned char*)malloc( displaceTex.getDataSize() );
	displaceTex.getData( displaceTexData );

	GLuint displaceTexture;
	glGenTextures(1, &displaceTexture);
	glBindTexture(GL_TEXTURE_2D, displaceTexture);

	glTexStorage2D( GL_TEXTURE_2D, 5, GL_RGB8, displaceTex.getWidth(), displaceTex.getHeight() );
	glTexSubImage2D( GL_TEXTURE_2D,0,0,0, displaceTex.getWidth(), displaceTex.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, displaceTexData );

	glGenerateMipmap( GL_TEXTURE_2D );

	textureMap.insert( std::pair< std::string, GLuint >( "DisplaceTest", displaceTexture ) );

	

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

bool GraphicsEngine::BufferMesh( std::string meshPath, GEVertex* mesh, unsigned int numVerts, GLushort* vertIndices, unsigned int numIndices )
{
	// Create GLuint to hold new voa.  Each Mesh gets its own voa.
	GLuint newVertexArrayObject;
	
	// Create a new buffers
	GLuint newVertexBuffer;
	GLuint newIndexBuffer;
	GLuint newIndirectBuffer;

	// Create the Vertex
	
	glGenVertexArrays( 1, &newVertexArrayObject );
	glBindVertexArray( newVertexArrayObject );

	glGenBuffers( 1, &newVertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, newVertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GEVertex ) * numVerts, mesh, GL_STATIC_DRAW );

	// Position
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, x ));
	glEnableVertexAttribArray(0);
	
	// Color
	glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, r ));
	glEnableVertexAttribArray(1);

	// Normals
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, nx ));
	glEnableVertexAttribArray(2);
	
	// Texture Coords.

	
	

	// Create the Index Buffer
	
	glGenBuffers( 1, &newIndexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, newIndexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( GLushort ) * numIndices, vertIndices, GL_STATIC_DRAW ); 

	// create the indirect buffer
	DrawElementsIndirectCommand indirectBufferData;

	indirectBufferData.baseInstance = 0;
	indirectBufferData.baseVertex = 0;
	indirectBufferData.firstIndex = 0;
	indirectBufferData.instanceCount = 1;
	indirectBufferData.elemCount = numIndices; 

	glGenBuffers( 1, &newIndirectBuffer );
	glBindBuffer( GL_DRAW_INDIRECT_BUFFER, newIndirectBuffer );
	glBufferData( GL_DRAW_INDIRECT_BUFFER, sizeof(indirectBufferData), &indirectBufferData, GL_STATIC_DRAW );
	

	// Next put mesh into the map

	GEMesh newMesh( GL_TRIANGLE_STRIP, numVerts, numIndices, newVertexArrayObject, newVertexBuffer, newIndexBuffer, newIndirectBuffer );  // TODO: where does GL_TRIAnGLES come from?
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

void GraphicsEngine::QueueInputItem( InputItem input )
{
	InputList.push( input );
}

