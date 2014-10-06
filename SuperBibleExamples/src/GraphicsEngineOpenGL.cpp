

#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GraphicsEngineOpenGL.h"
#include "GameEngine.h"
#include "TextureManager.h"
#include "ImageUtilities\ImageUtilities.h"
#include "CameraPerspective.h"
#include "InfoViewport.h"
#include "InfoGameVars.h"
#include "GEInputState.h"


GraphicsEngineOpenGL::GraphicsEngineOpenGL()
{
	seed = 0x13371337;  //here just for the alien rain example

	//Initialize Graphics upon instantiation.  TODO Might want to make the program make an explicit call.  For now this works
	if(!Init())
	{
		printf("Error Initializing Graphics");
		exit(EXIT_FAILURE);
	}

	
}

GraphicsEngineOpenGL::GraphicsEngineOpenGL( const GEObjectContainer* gameEntities )
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

GraphicsEngineOpenGL::~GraphicsEngineOpenGL()
{
	// clean up all pointers here

	//voa

	//vb
}

// setters

// getters


// this version of render is for tutorial code.
void GraphicsEngineOpenGL::Render(const double currentTime)
{
	glEnable ( GL_STENCIL_TEST );

	// get the viewport info out of the game entities
	const InfoViewport* viewportInfo = (InfoViewport*)gameEntities->GetObject( "SYS_Viewport_Options" );

	if ( viewportInfo != nullptr )
	{
		// set render region
		//glViewport(0,0,viewportInfo->getViewportWidth() , viewportInfo->getViewportHeight() );
		glViewportIndexedf(0, 0, 0, viewportInfo->getViewportWidth(), viewportInfo->getViewportHeight());

		// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
		glm::mat4 viewMatrix;
		
		// find the rendercam
		const CameraObject* renderCam = (CameraObject*)gameEntities->GetObject( viewportInfo->getRenderCam() );

		if( renderCam != nullptr )
		{
			if (renderCam->getClassName() == "CameraPerspective" )
			{
				viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFinalFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f) * renderCam->GetViewMatrix();
			}
		}
		else
		{
			// if can't find renderCam build a generic one.
			renderCam = new CameraPerspective( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), glm::radians( 45.0f ) );
			viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFinalFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f) * renderCam->GetViewMatrix();
		}
	
		const GLfloat bkColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		const GLfloat one = 1.0f;
		const GLfloat zero = 0.0f; 

		glClearBufferfv( GL_COLOR, 0, bkColor );
		glClearBufferfv( GL_DEPTH,0, &one );
		glClearBufferfv( GL_STENCIL, 0, &zero );

		//std::map< std::string, GEMesh>::const_iterator meshIt = meshMap.find("sphere");

		//GEMesh renderMesh = meshIt->second;
		GEMesh renderMesh = resMesh.GetResource( "plane" );

		glBindVertexArray( renderMesh.getVertexArrayObject() );
	
		GEMaterial renderMaterial;
		
		renderMaterial = resMaterial.GetResource( "default_wTexture" );

		glUseProgram ( renderMaterial.getProgram() );
			
		GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "worldMatrix" );
		GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "viewMatrix" );
		//GLint tessLevelLocation = glGetUniformLocation( renderMaterial.getProgram(), "tessLevel" );
		//GLint displaceTextureLoc = glGetUniformLocation( renderMaterial.getProgram(), "displaceTexture");

		//bind the displace texture
		//glBindTexture( GL_TEXTURE_2D, textureMap.find("DisplaceTest")->second );

		//glm::mat4 worldMatrix = it->second->GetTransformMatrix();
		glm::mat4 worldMatrix = glm::rotate( glm::mat4(),(float)currentTime / 4.0f,GEvec3(0.0f, 1.0f, 0.0f ) )* glm::scale( glm::mat4(), GEvec3(1.5f, 1.5f, 1.5f));

		glUniformMatrix4fv( worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0] );
		glUniformMatrix4fv( viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0] );
	
		// bind the required textures
		for( unsigned int i = 0; i < renderMaterial.getTextures().size(); i++ )
		{
			glActiveTexture( GL_TEXTURE0 + i );
			GLuint renderTex = resTexture.GetResource( renderMaterial.getTextures()[i] );
			glBindTexture( GL_TEXTURE_2D, renderTex );

			std::string textureString = "texture";
			textureString.append( std::to_string(i) );

			GLint texLoc = glGetUniformLocation( renderMaterial.getProgram(), textureString.c_str() );
			glUniform1i( texLoc, i );

		}
		glStencilFunc( GL_ALWAYS, 1, 0xff );
		glStencilOp( GL_KEEP, GL_ZERO, GL_REPLACE );

		glEnable( GL_PRIMITIVE_RESTART );
		glPrimitiveRestartIndex( 0xFFFF );
		glDrawElements( GL_TRIANGLE_STRIP,renderMesh.getNumIndices(),GL_UNSIGNED_INT, 0 );
		glDisable( GL_PRIMITIVE_RESTART );

		
		// draw second time reversed rotation

		// clear the color and depth buffers... keeping the stencil
		glClearBufferfv( GL_COLOR, 0, bkColor );
		glClearBufferfv( GL_DEPTH, 0, &one );

		// change the stencil ops
		glStencilFunc( GL_EQUAL, 1, 0xff );
		glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );

		worldMatrix = glm::rotate( glm::mat4(),(float)currentTime / -4.0f,GEvec3(0.0f, 1.0f, 0.0f ) )* glm::scale( glm::mat4(), GEvec3(1.5f, 1.5f, 1.5f));

		glUniformMatrix4fv( worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0] );
		glUniformMatrix4fv( viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0] );
	
		// bind the required textures
		for( unsigned int i = 0; i < renderMaterial.getTextures().size(); i++ )
		{
			glActiveTexture( GL_TEXTURE0 + i );
			GLuint renderTex = resTexture.GetResource( renderMaterial.getTextures()[i] );
			glBindTexture( GL_TEXTURE_2D, renderTex );

			std::string textureString = "texture";
			textureString.append( std::to_string(i) );

			GLint texLoc = glGetUniformLocation( renderMaterial.getProgram(), textureString.c_str() );
			glUniform1i( texLoc, i );

		}
		glEnable( GL_PRIMITIVE_RESTART );
		glPrimitiveRestartIndex( 0xFFFF );
		glDrawElements( GL_TRIANGLE_STRIP,renderMesh.getNumIndices(),GL_UNSIGNED_INT, 0 );
		glDisable( GL_PRIMITIVE_RESTART );

		
	
		
	}
	
	glDisable ( GL_STENCIL_TEST );


	RenderFPS( currentTime );

	glfwSwapBuffers(window);
	glfwPollEvents();
}

// this version of render is for the actual game engine.
void GraphicsEngineOpenGL::Render(const double currentTime, const GEObjectContainer* gameEntities_notused)
{
	
	// get the viewport info out of the game entities
	const InfoViewport* viewportInfo = (InfoViewport*)gameEntities->GetObject( "SYS_Viewport_Options" );
	
	if ( viewportInfo != nullptr )
	{
		// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
		glm::mat4 viewMatrix;

		// find the rendercam
		const CameraObject* renderCam = (CameraObject*)gameEntities->GetObject( viewportInfo->getRenderCam() );

		if( renderCam != nullptr )
		{
			if (renderCam->getClassName() == "CameraPerspective" )
			{
				viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFinalFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 1000.0f) * renderCam->GetViewMatrix();
			}
		}
		else
		{
			// if can't find renderCam build a generic one.
			renderCam = new CameraPerspective( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), glm::radians( 45.0f ) );
			viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFinalFov(), (float)viewportInfo->getViewportWidth()/(float)viewportInfo->getViewportHeight(), 0.1f, 4.0f) * renderCam->GetViewMatrix();
		}

		const GLfloat bkColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		const GLfloat one = 1.0f;
	
		glClearBufferfv(GL_COLOR, 0, bkColor);
		glClearBufferfv(GL_DEPTH,0, &one);

		// Iterate throught game entities here.
	
		for ( std::map< std::string, GEObject* >::const_iterator it = gameEntities->First(); it != gameEntities->Last(); it++ )
		{
			if ( it->second->isVisible() )
			{
				GEMaterial renderMaterial;
				std::string renderMaterialName = it->second->getMaterial();

				renderMaterial = resMaterial.GetResource( renderMaterialName );
			
				glUseProgram ( renderMaterial.getProgram() );
			
				GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "worldMatrix" );
				GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "viewMatrix" );

				GEMesh renderMesh = resMesh.GetResource( it->second->getMesh() );

				glBindVertexArray( renderMesh.getVertexArrayObject() );
			
				glm::mat4 worldMatrix = it->second->GetTransformMatrix();

				glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
				glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            
				// bind the required textures
				for( unsigned int i = 0; i < renderMaterial.getTextures().size(); i++ )
				{
					glActiveTexture( GL_TEXTURE0 + i );
					GLuint renderTex = resTexture.GetResource( renderMaterial.getTextures()[i] );
					glBindTexture( GL_TEXTURE_2D, renderTex );

					std::string textureString = "texture";
					textureString.append( std::to_string(i) );

					GLint texLoc = glGetUniformLocation( renderMaterial.getProgram(), textureString.c_str() );
					glUniform1i( texLoc, i );

				}



				glEnable( GL_PRIMITIVE_RESTART );
				glPrimitiveRestartIndex( 0xFFFF );
				for (int j = 0; j<1; j++ )  // just to test performance
				{
					//glDrawElements( renderMesh.getMeshType(), renderMesh.getNumIndices(), GL_UNSIGNED_SHORT, 0);
					glBindBuffer( GL_DRAW_INDIRECT_BUFFER, renderMesh.getIndirectBuffer() );
					glDrawElementsIndirect( renderMesh.getMeshType(), GL_UNSIGNED_INT, 0);
					//glDrawElementsInstanced(  renderMesh.getMeshType(), renderMesh.getNumIndices(), GL_UNSIGNED_SHORT, 0, 10000);
				}
				glDisable( GL_PRIMITIVE_RESTART );
				
			}
	
		}
	}
	
	RenderFPS( currentTime );

	

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GraphicsEngineOpenGL::RenderFPS(const double currentTime)
{
	// Testing text output

	// get the viewport info out of the game entities
	const InfoViewport* viewportInfo = (InfoViewport*)gameEntities->GetObject( "SYS_Viewport_Options" );
	
	if ( viewportInfo != nullptr )
	{
		glViewportIndexedf(0, 0, 0, viewportInfo->getViewportWidth(), viewportInfo->getViewportHeight());

		GEMesh fontMesh = resMesh.GetResource( "SYS_FONT" );
	
		glBindVertexArray( fontMesh.getVertexArrayObject() );

		GEMaterial fontMaterial = resMaterial.GetResource( "SystemFont01" );

		glUseProgram( fontMaterial.getProgram() );
		glActiveTexture( GL_TEXTURE0 );

		GLint fontTextureLoc = glGetUniformLocation( fontMaterial.getProgram(), "fontTexture");
		GLint screenMatrixLocation = glGetUniformLocation( fontMaterial.getProgram(), "screenMatrix" );
		GLuint charSizeLoc = glGetUniformLocation( fontMaterial.getProgram(), "charSize" );
		GLuint startPosLoc = glGetUniformLocation( fontMaterial.getProgram(), "startPos" );
		GLuint fontColorLoc = glGetUniformLocation( fontMaterial.getProgram(), "fontColor" );
	
		glUniform1f( charSizeLoc, 0.015f );
		glUniform2f( startPosLoc, 0.0075f, 0.0075f );
		glUniform4f( fontColorLoc, 1.0f, 0.0f, 0.0f, 1.0f );

		glm::mat4 screenMatrix = glm::ortho( 0.0f, 1.0f, (float)viewportInfo->getViewportHeight()/(float)viewportInfo->getViewportWidth(), 0.0f );

		glUniformMatrix4fv(screenMatrixLocation, 1, GL_FALSE, &screenMatrix[0][0]);
	
		glUniform1i(fontTextureLoc, 0);

		glBindTexture( GL_TEXTURE_2D, resTexture.GetResource( "SYS_Font01" ) );
		
		const InfoGameVars* gameVars = (InfoGameVars*)gameEntities->GetObject( "SYS_Game_Vars" );

		std::string testString1, testString2;

		if( gameVars != nullptr )
		{
			testString1 = "FPS: ";
			testString2 = std::to_string( 1.0 / gameVars->getDeltaFrameTime() );
			testString1.append( testString2 );
		}
		else
		{
			testString1 = "Error: Cannot find SYS_Game_Vars";
		}
	
		glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
		glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar)*testString1.length(), testString1.c_str());
		glVertexAttribDivisor(0, 1);
		glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, testString1.length() );
		glVertexAttribDivisor(0, 0);

		// temp render the pressed inputs.------------------------------------------

		// get pointer to the input state
		const GEInputState* inputState = (GEInputState*)gameEntities->GetObject( "SYS_Input_State" );
		

		std::string inputString = "";
		
		if( inputState != nullptr )
		{
			for ( unsigned int i = 0; i < INPUTSTATE_MAX_KEY_BUTTONS; i++ )
			{
				if (inputState->getKeyboardKey( i ) )  // if key pressed
				{
					inputString.append( inputState->KeyToString( i ) );  // get the string representation of it
					inputString.append( " " );
				}
			}
		}
		else
		{
			inputString = "Error: Cannot find SYS_Input_State";
		}

		glUniform2f( startPosLoc, 0.0075f, 0.04f );
		glUniform4f( fontColorLoc, 0.0f, 1.0f, 0.0f, 1.0f );
		glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
		glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar) * inputString.length(), inputString.c_str());
		glVertexAttribDivisor(0, 1);
		glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, inputString.length() );
		glVertexAttribDivisor(0, 0);

		// now render the mouse position
		inputString = "";
	
		inputString = std::to_string( (int)inputState->getMousePosition().x ) + ", " + std::to_string( (int)inputState->getMousePosition().y );

		glUniform2f( startPosLoc, 0.0075f, 0.0275f );
		glUniform4f( fontColorLoc, 0.0f, 1.0f, 0.0f, 1.0f );
		glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
		glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar) * inputString.length(), inputString.c_str());
		glVertexAttribDivisor(0, 1);
		glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, inputString.length() );
		glVertexAttribDivisor(0, 0);

		// now the mouse buttons

		inputString = "";
	
		for ( unsigned int i = 0; i < INPUTSTATE_MAX_MOUSE_BUTTONS; i++ )
		{
			if (inputState->getMouseButton( i ) )  // if key pressed
			{
				inputString.append( inputState->ButtonToString( i ) );  // get the string representation of it
				inputString.append( " " );
			}
		}

		glUniform2f( startPosLoc, 0.0075f, 0.05f );
		glUniform4f( fontColorLoc, 0.0f, 1.0f, 0.0f, 1.0f );
		glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
		glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar) * inputString.length(), inputString.c_str());
		glVertexAttribDivisor(0, 1);
		glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, inputString.length() );
		glVertexAttribDivisor(0, 0);

		// now the mouse scroll

		inputString = "";
	
		inputString = std::to_string( (int)inputState->getMouseScrollOffset().x ) + ", " + std::to_string( (int)inputState->getMouseScrollOffset().y );

		glUniform2f( startPosLoc, 0.0075f, 0.06f );
		glUniform4f( fontColorLoc, 0.0f, 1.0f, 0.0f, 1.0f );
		glBindBuffer( GL_ARRAY_BUFFER, fontMesh.getVertexBuffer() );  // need to manually bind the buffer if we are altering it.
		glBufferSubData( GL_ARRAY_BUFFER, 0,sizeof(GLchar) * inputString.length(), inputString.c_str());
		glVertexAttribDivisor(0, 1);
		glDrawArraysInstanced( GL_TRIANGLE_STRIP, 0, 4, inputString.length() );
		glVertexAttribDivisor(0, 0);
	}
}

bool GraphicsEngineOpenGL::Init()
{
	bool success = false;

	glfwSetErrorCallback(error_callback);

	// initial OpenGL (glfw)
	if( !glfwInit() )
		return success;		// success is still false here.
	
	const InfoViewport* viewportInfo = (InfoViewport*)gameEntities->GetObject( "SYS_Viewport_Options" );

	if ( viewportInfo != nullptr )
	{

	
		//	Added so that OSX would use OpenGL 3.2 instead of the default 2.1
		/**
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		*/

		// set window hints
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		glfwWindowHint( GLFW_STENCIL_BITS, GL_TRUE );

		// create the window
		window = glfwCreateWindow( viewportInfo->getViewportWidth(), viewportInfo->getViewportHeight(), "OpenGL Super Bible", NULL,NULL);
		if(!window)
		{
			glfwTerminate();
			return false;
			//exit(EXIT_FAILURE);
		}
	
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
		glfwSetKeyCallback( window, key_callback );
		glfwSetWindowSizeCallback( window, window_size_callback );
		glfwSetCursorPos( window, viewportInfo->getViewportWidth() / 2, viewportInfo->getViewportHeight() / 2 );
		glfwSetCursorPosCallback( window, mouse_position_callback );
		glfwSetMouseButtonCallback( window, mouse_button_callback );
		glfwSetScrollCallback( window, mouse_scroll_callback );


		// set the window pointer to this graphics engine so the callback functions have access to it.
		glfwSetWindowUserPointer( window, this ); 	
	
		//not sure this is the best place for these, but will work for now
		InitShaders();
		InitBuffers();
		InitTextures();
	
		//Set some options
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);

		success = true;		// if we got here all must have gone well.

	}

	
	return success;
}

bool GraphicsEngineOpenGL::CleanUp()
{

	glfwDestroyWindow(window);
	glfwTerminate();

	return true;  //eventually change to success or fail.
}


int GraphicsEngineOpenGL::CheckWindowClose()
{
	return glfwWindowShouldClose(window);
}

double GraphicsEngineOpenGL::getCurrentTime() const
{
	return glfwGetTime();
}

bool GraphicsEngineOpenGL::InitShaders(void)
{
	// start by adding the "GE_MISSING" shader

	GLuint program;
	GLuint vertexShader;
	GLuint fragmentShader;

	program = glCreateProgram();

	const char* vsCode[] = 
	{
		"#version 430\n"

		"layout (location = 0) in vec4 position;\n"
		"layout (location = 1) in vec4 color;\n"
		"layout (location = 2) in vec3 normal;\n"
		"layout (location = 3) in vec2 tc;\n"


		"out VS_OUT\n"
		"{"
		"	vec4 color; \n"
		"	vec3 normal;\n"
		"	vec2 tc;\n"
		"} vs_out;\n"

		"uniform mat4 worldMatrix;\n"
		"uniform mat4 viewMatrix;\n"

		"void main(void)"
		"{"
		"	gl_Position =  viewMatrix * worldMatrix * position;\n"
		"	vs_out.color = color;\n"
		"	vs_out.normal = normal;\n"
		"	vs_out.tc = tc;\n"
		"}"
	};

	vertexShader = materialMan.CompileShaderFromSource( vsCode, GL_VERTEX_SHADER );

	const char* fsCode[] = 
	{
		"#version 430  \n"
                                                                            
        "layout (location = 0) out vec4 color;\n"
        "                                                                       \n"
        "in VS_OUT                                                              \n"
        "{                                                                      \n"
        "	vec4 color; \n"
		"	vec3 normal;\n"
		"	vec2 tc;\n"
        "} fs_in;                                                               \n"
        
        "void main(void)                                                        \n"
        "{                                                                      \n"
		"    color = vec4(1.0,1.0,0.0,1.0);   \n"
        "}\n"      
	};

	fragmentShader = materialMan.CompileShaderFromSource( fsCode, GL_FRAGMENT_SHADER );

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glLinkProgram( program ); // TODO: create a link program function in material man.

	GLint return_code;

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

		//success = false;
	}

	GEMaterial missingMat;
	missingMat.setProgram( program );

	resMaterial.AddResource( GE_MISSING, missingMat );

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	BufferMaterial( "alien_rain" );
	BufferMaterial( "SolidRGB_Subroutine" );
	BufferMaterial( "SystemFont01" );
	BufferMaterial( "texture_test" );

	// Manual buffer some default shaders
	

	return true;
}
/**
	Place to experiment with Buffers
*/
void GraphicsEngineOpenGL::InitBuffers(void)
{
	GLuint buffer[2];

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
	//meshMap["SYS_FONT"] = newMesh;
	resMesh.AddResource( "SYS_FONT", newMesh );
	

}

void GraphicsEngineOpenGL::InitTextures(void)
{
	
	// create the GE_MISSING texture
	GLuint missingTexture = textureMan.LoadTexture( GE_MISSING );
	
	if( missingTexture != 0 )
		resTexture.AddResource( GE_MISSING, missingTexture );

	GLenum glError = 0;
	unsigned char returnType = 0;

	// Load the font Texture
	GLuint fontTexture = textureMan.LoadTexture( "font01.bmp" );
	
	if( fontTexture != 0 )
		resTexture.AddResource( "SYS_Font01", fontTexture );
	
	// Load the displace Texture (testing)
	GLuint displaceTexture = textureMan.LoadTexture( "displace_test.bmp" );
	
	if( displaceTexture != 0 )
		resTexture.AddResource( "DisplaceTest", displaceTexture );

	//	now we generate the sampler... optional (a default sampler will be assigned otherwise)
	GLuint sampler;
	glGenSamplers(1, &sampler);

	//	set some sampler options
	glSamplerParameteri(sampler,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glSamplerParameteri(sampler,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	//	now bind it to a texture unit
	glBindSampler(0, sampler);	//bind it to texture unit 0... the only one we are using currently.

}

void GraphicsEngineOpenGL::UpdateWindowSize(int x, int y, int width, int height)
{
	float aspect = (float)width/(float)height;
	glViewport(x,y,width,height);
}

bool GraphicsEngineOpenGL::isMeshBuffered( std::string meshPath)
{
	// check if mesh already loaded
	return resMesh.ContainsResource( meshPath );
}

bool GraphicsEngineOpenGL::isMaterialBuffered( std::string materialPath )
{
	// check if material already loaded
	return resMaterial.ContainsResource( materialPath );
}

bool GraphicsEngineOpenGL::isTextureBuffered( std::string texturePath )
{
	// check if texture already loaded
	return resTexture.ContainsResource( texturePath );
}

bool GraphicsEngineOpenGL::BufferMesh( std::string meshPath, MUMesh* mesh )
{
	// Create GLuint to hold new voa.  Each Mesh gets its own voa.
	GLuint newVertexArrayObject;
	
	// Create new buffers
	GLuint newVertexBuffer;
	GLuint newIndexBuffer;
	GLuint newIndirectBuffer;

	// Create the Vertex
	
	glGenVertexArrays( 1, &newVertexArrayObject );
	glBindVertexArray( newVertexArrayObject );

	glGenBuffers( 1, &newVertexBuffer );
	glBindBuffer( GL_ARRAY_BUFFER, newVertexBuffer );
	glBufferData( GL_ARRAY_BUFFER, sizeof( GEVertex ) * mesh->getNumVerts(), mesh->getVerticies(), GL_STATIC_DRAW );

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
	glVertexAttribPointer( 3, 2, GL_FLOAT, GL_FALSE, sizeof( GEVertex ), ( void* )offsetof( GEVertex, u ));
	glEnableVertexAttribArray(3);
	
	

	// Create the Index Buffer
	
	glGenBuffers( 1, &newIndexBuffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, newIndexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( unsigned int ) * mesh->getNumIndicies(), mesh->getIndicies(), GL_STATIC_DRAW ); 

	// create the indirect buffer
	DrawElementsIndirectCommand indirectBufferData;

	indirectBufferData.baseInstance = 0;
	indirectBufferData.baseVertex = 0;
	indirectBufferData.firstIndex = 0;
	indirectBufferData.instanceCount = 1;
	indirectBufferData.elemCount = mesh->getNumIndicies(); 

	glGenBuffers( 1, &newIndirectBuffer );
	glBindBuffer( GL_DRAW_INDIRECT_BUFFER, newIndirectBuffer );
	glBufferData( GL_DRAW_INDIRECT_BUFFER, sizeof(indirectBufferData), &indirectBufferData, GL_STATIC_DRAW );
	

	// Next put mesh into the map

	unsigned int meshType;

	if( mesh->getMeshType() == GE_MESH_TRIANGLE_LIST )
		meshType = GL_TRIANGLES;
	else if( mesh->getMeshType() == GE_MESH_TRIANGLE_STRIP )
		meshType = GL_TRIANGLE_STRIP;
	else
		meshType = GL_TRIANGLES;

	GEMesh newMesh( meshType, mesh->getNumVerts(), mesh->getNumIndicies(), newVertexArrayObject, newVertexBuffer, newIndexBuffer, newIndirectBuffer );
	//meshMap[meshPath] = newMesh;
	resMesh.AddResource( meshPath, newMesh );

	// Unbind voa??  Don't for now.

	return true;
}

bool GraphicsEngineOpenGL::BufferMaterial( std::string materialPath )
{
	bool success = false; 

	printf( "Buffering Material: %s\n====================================\n", materialPath.c_str() );

	GEMaterial newMaterial = materialMan.LoadMaterial( materialPath );

	// Verify the material loaded

	if ( newMaterial.getProgram() != 0 )
	{
		// Add it to the shader map
	 
		//materialMap.insert( std::pair< std::string, GEMaterial >( materialPath, newMaterial ) );
		resMaterial.AddResource( materialPath, newMaterial );

		// Next load the textures.
		for( unsigned int i = 0; i < newMaterial.getTextures().size(); i++ )
		{
			bool buffered;

			printf( "Loading Texture: %s...", newMaterial.getTextures()[i].c_str() );

			buffered = BufferTexture( newMaterial.getTextures()[i] );

			// check if texture loaded.  don't change success as its not the 
			// the end of the world if the texture does not load.  
			if (buffered)
				printf( "Success\n" );
			else
				printf( "Failed\n" );

		}

		success = true;

		printf ( "Buffering Complete\n");
	}
	else
	{
		printf ( "Buffering Failed\n");
	}


	return success;
}

bool GraphicsEngineOpenGL::BufferTexture( std::string texturePath )
{
	bool success = false;

	if( !isTextureBuffered( texturePath ) )
	{
		// load the texture
		GLuint texture = textureMan.LoadTexture( texturePath );

		// then put that texture into the textureMap.
		if ( texture != 0 )
		{
			resTexture.AddResource( texturePath, texture);
			success = true;
		}
	}
	else
		success = true;

	return success;
}


