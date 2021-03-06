

#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GraphicsEngineOpenGL.h"
#include "GameEngine.h"
#include "TextureManager.h"
#include "ImageUtilities\ImageUtilities.h"
#include "CameraPerspective.h"
#include "InfoGameVars.h"
#include "InfoGameEngineSettings.h"
#include "GEInputState.h"
#include "InputStateHolder.h"
#include "GEBoundingBox.h"
#include "GERay.h"


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
void GraphicsEngineOpenGL::RenderTut(const double currentTime)
{
	

	// get the viewport info out of the game entities
	const InfoGameEngineSettings* gameEngineSettings = (InfoGameEngineSettings*)gameEntities->GetObject( "SYS_GameEngine_Settings" );

	if ( gameEngineSettings != nullptr )
	{
		const GLfloat bkColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		const GLfloat green[] = { 0.0f, 1.0f, 0.0f, 1.0f };
		const GLfloat one = 1.0f;
		const GLfloat zero = 0.0f; 

		// Bind our FBO
		glBindFramebuffer( GL_FRAMEBUFFER, fbo );

		// Set the fiewport and clear the depth and color buffers
		glViewport( 0, 0, 512, 512 );
		glClearBufferfv( GL_COLOR, 0, green );
		glClearBufferfv( GL_DEPTH, 0, &one );

		// calculate the view matrix... which is constant for all objects... only need to calc once per frame.
		glm::mat4 viewMatrix;
		
		// create a camera.
		const CameraObject* renderCam =  new CameraPerspective( GEvec3( 0.0f, 0.0f, 5.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), glm::radians( 45.0f ) );
		viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFovFinal( gameEntities ), 1.0f, 0.1f, 1000.0f) * renderCam->GetViewMatrix( gameEntities );
		
		GEMesh renderMesh = resMesh.GetResource( "plane" );

		glBindVertexArray( renderMesh.getVertexArrayObject() );
	
		GEMaterial renderMaterial;
		
		renderMaterial = resMaterial.GetResource( "default_wTexture" );

		glUseProgram ( renderMaterial.getProgram() );
			
		GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "worldMatrix" );
		GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "viewMatrix" );
		
		glm::mat4 worldMatrix = glm::rotate( glm::mat4(),(float)currentTime / 1.0f,GEvec3(0.0f, 1.0f, 0.0f ) )* glm::scale( glm::mat4(), GEvec3(1.5f, 1.5f, 1.5f));

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
		glDrawElements( renderMesh.getMeshType(), renderMesh.getNumIndices(),GL_UNSIGNED_INT, 0 );
		glDisable( GL_PRIMITIVE_RESTART );

		// cleanup

		delete renderCam;
		
		// draw a cube with the rendered texture.

		// return to default framebuffer
		glBindFramebuffer( GL_FRAMEBUFFER, 0 );

		// create a camera and new view matrix
		renderCam =  new CameraPerspective( GEvec3( 0.0f, 0.0f, 5.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), glm::radians( 45.0f ) );
		viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFovFinal( gameEntities ), (float)gameEngineSettings->getViewportWidth()/(float)gameEngineSettings->getViewportHeight(), 0.1f, 1000.0f) * renderCam->GetViewMatrix( gameEntities );

		// reset the viewport
		glViewport( 0, 0,  (float)gameEngineSettings->getViewportWidth(), (float)gameEngineSettings->getViewportHeight() );

		// clear the color and depth buffers
		glClearBufferfv( GL_COLOR, 0, bkColor );
		glClearBufferfv( GL_DEPTH, 0, &one );

		

		worldMatrix = glm::rotate( glm::mat4(),(float)currentTime / -4.0f,GEvec3(1.0f, 1.0f, 0.0f ) )* glm::scale( glm::mat4(), GEvec3(1.5f, 1.5f, 1.5f));

		glUniformMatrix4fv( worldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0] );
		glUniformMatrix4fv( viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0] );
	
		// bind the required textures
		/*for( unsigned int i = 0; i < renderMaterial.getTextures().size(); i++ )
		{
			glActiveTexture( GL_TEXTURE0 + i );
			GLuint renderTex = resTexture.GetResource( renderMaterial.getTextures()[i] );
			glBindTexture( GL_TEXTURE_2D, renderTex );

			std::string textureString = "texture";
			textureString.append( std::to_string(i) );

			GLint texLoc = glGetUniformLocation( renderMaterial.getProgram(), textureString.c_str() );
			glUniform1i( texLoc, i );

		}*/

		renderMesh = resMesh.GetResource( "testBox" );

		glBindVertexArray( renderMesh.getVertexArrayObject() );

		// bind the texture we just wrote to.
		glActiveTexture( GL_TEXTURE0 );
		glBindTexture( GL_TEXTURE_2D, color_texture );

		glEnable( GL_PRIMITIVE_RESTART );
		glPrimitiveRestartIndex( 0xFFFF );
		glDrawElements( renderMesh.getMeshType(),renderMesh.getNumIndices(),GL_UNSIGNED_INT, 0 );
		glDisable( GL_PRIMITIVE_RESTART );
		
		
		// clean up
		delete renderCam;
		
	}

	RenderFPS( currentTime );

	glfwSwapBuffers(window);
	glfwPollEvents();
}

// this version of render is for the actual game engine.
void GraphicsEngineOpenGL::Render( const double currentTime )
{
	
	// get the viewport info and gameengine settings out of the game entities
	const InfoGameEngineSettings* gameEngineSettings = (InfoGameEngineSettings*)gameEntities->GetObject( "SYS_GameEngine_Settings" );
	
	if ( gameEngineSettings != nullptr )
	{
		// declare some variables
		glm::mat4 viewMatrix;
		GEMaterial boundingMaterial = resMaterial.GetResource( "boundingBox" );										// get bounding box material incase we are showing that.
		GLint boundingworldMatrixLocation = glGetUniformLocation( boundingMaterial.getProgram(), "worldMatrix" );
		GLint boundingviewMatrixLocation = glGetUniformLocation( boundingMaterial.getProgram(), "viewMatrix" );
		GLint boundingMinLocation = glGetUniformLocation( boundingMaterial.getProgram(), "bmin" );
		GLint boundingMaxLocation = glGetUniformLocation( boundingMaterial.getProgram(), "bmax" );

		// calculate the view matrix... which is constant for all objects... only need to calc once per frame.

		// find the rendercam
		const CameraObject* renderCam = (CameraObject*)gameEntities->GetObject( gameEngineSettings->getRenderCam() );

		if( renderCam != nullptr )
		{
			if (renderCam->getClassName() == "CameraPerspective" )
			{
				viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFovFinal( gameEntities ), (float)gameEngineSettings->getViewportWidth()/(float)gameEngineSettings->getViewportHeight(), 0.1f, 1000.0f) * renderCam->GetViewMatrix( gameEntities );
			}
		}
		else
		{
			// if can't find renderCam build a generic one.
			renderCam = new CameraPerspective( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), glm::radians( 45.0f ) );
			viewMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFovFinal( gameEntities ), (float)gameEngineSettings->getViewportWidth()/(float)gameEngineSettings->getViewportHeight(), 0.1f, 4.0f) * renderCam->GetViewMatrix( gameEntities );
		}

		const GLfloat bkColor[] = { 0.2f, 0.2f, 0.2f, 1.0f };
		const GLfloat one = 1.0f;
	
		glClearBufferfv(GL_COLOR, 0, bkColor);
		glClearBufferfv(GL_DEPTH,0, &one);

		// Set Render Modes

		// Set the draw mode
		switch ( gameEngineSettings->getRenderMode() )
		{
		case GE_RENDERMODE_WIRE:
			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
			break;
		case GE_RENDERMODE_FULL:
		default:
			glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		};

		// set the depth mode
		glDepthFunc( GL_LEQUAL );

		// set sampling
		if( gameEngineSettings->getEnableMultiSample() )
		{
			glEnable( GL_MULTISAMPLE );
		}
		else
			glDisable( GL_MULTISAMPLE );

		// Iterate throught game entities here.
	
		for ( std::map< std::string, GEObject* >::const_iterator it = gameEntities->First(); it != gameEntities->Last(); it++ )
		{
			if ( it->second->isVisible() )
			{
				if( it->first == "testOrbiter2" )
				{
					printf("test");
				}

				GEMaterial renderMaterial;
				std::string renderMaterialName = it->second->getMaterialValue();

				renderMaterial = resMaterial.GetResource( renderMaterialName );
			
				glUseProgram ( renderMaterial.getProgram() );
			
				GLint worldMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "worldMatrix" );
				GLint viewMatrixLocation = glGetUniformLocation( renderMaterial.getProgram(), "viewMatrix" );

				GEMesh renderMesh = resMesh.GetResource( it->second->getMesh() );

				glBindVertexArray( renderMesh.getVertexArrayObject() );
			
				glm::mat4 worldMatrix = it->second->GetTransformMatrix( gameEntities );

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

				// Now render the bounding box if necessary

				if( gameEngineSettings->getShowBoundingBoxes() )
				{
					GEBoundingBox boundingBox = renderMesh.getBoundingBox();
			
					glUseProgram ( boundingMaterial.getProgram() );

					glUniformMatrix4fv( boundingworldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0] );
					glUniformMatrix4fv( boundingviewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0] );

					glUniform3fv( boundingMinLocation, 1, &boundingBox.getMin()[0] );
					glUniform3fv( boundingMaxLocation, 1, &boundingBox.getMax()[0] );

					glDrawArrays( GL_LINES, 0, 24 );

				}
				
			}
	
		}

		// unset render modoes
		glDisable( GL_MULTISAMPLE );
	}
	// TODO... what happens if nullptr?
	
	RenderFPS( currentTime );

	

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void GraphicsEngineOpenGL::RenderFPS(const double currentTime)
{
	// Testing text output



	// get the viewport info out of the game entities
	const InfoGameEngineSettings* gameEngineSettings = (InfoGameEngineSettings*)gameEntities->GetObject( "SYS_GameEngine_Settings" );
	
	if ( gameEngineSettings != nullptr )
	{
		// always draw text filled
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
		glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

		glViewportIndexedf(0, 0, 0, gameEngineSettings->getViewportWidth(), gameEngineSettings->getViewportHeight());

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

		glm::mat4 screenMatrix = glm::ortho( 0.0f, 1.0f, (float)gameEngineSettings->getViewportHeight()/(float)gameEngineSettings->getViewportWidth(), 0.0f );

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
		const GEObject* isObject = gameEntities->GetObject( "SYS_Input_State" );
	
		if ( isObject != nullptr )
		{
			const InputStateHolder* inputStateHolder = (InputStateHolder*)isObject;
			const GEInputState* inputState = inputStateHolder->getInputState();

			std::string inputString = "";
		
			for ( unsigned int i = 0; i < INPUTSTATE_MAX_KEY_BUTTONS; i++ )
			{
				if (inputState->getKeyboardKey( i ) )  // if key pressed
				{
					inputString.append( inputState->KeyToString( i ) );  // get the string representation of it
					inputString.append( " " );
				}
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

			glDisable( GL_BLEND );
		}
		
	}
}

bool GraphicsEngineOpenGL::Init()
{
	bool success = false;

	glfwSetErrorCallback(error_callback);

	// initial OpenGL (glfw)
	if( !glfwInit() )
		return success;		// success is still false here.
	
	const InfoGameEngineSettings* gameEngineSettings = (InfoGameEngineSettings*)gameEntities->GetObject( "SYS_GameEngine_Settings" );

	if ( gameEngineSettings != nullptr )
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
		glfwWindowHint( GLFW_SAMPLES, 32 );

		// create the window
		window = glfwCreateWindow( gameEngineSettings->getViewportWidth(), gameEngineSettings->getViewportHeight(), "OpenGL Super Bible", NULL,NULL);
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
		glfwSetCursorPos( window, gameEngineSettings->getViewportWidth() / 2, gameEngineSettings->getViewportHeight() / 2 );
		glfwSetCursorPosCallback( window, mouse_position_callback );
		glfwSetMouseButtonCallback( window, mouse_button_callback );
		glfwSetScrollCallback( window, mouse_scroll_callback );

		// set vsync on
		glfwSwapInterval( 1 );

		// set the window pointer to this graphics engine so the callback functions have access to it.
		glfwSetWindowUserPointer( window, this ); 	
		SetMouseMode( gameEngineSettings->getMouseMode() );
	
		//not sure this is the best place for these, but will work for now
		InitShaders();
		InitBuffers();
		InitTextures();
	
		//Set some options
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);


		success = true;		// if we got here all must have gone well.

	}

	// Some random stuff that can be removed
	GLint texSize;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &texSize );


	
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

	GEMesh newMesh( GL_TRIANGLES, 0, 0, fontVOA, fontVBO, 0,0, GEBoundingBox() );  // the font mesh doesn't really fit the GEMesh type... new type?
	//meshMap["SYS_FONT"] = newMesh;
	resMesh.AddResource( "SYS_FONT", newMesh );

	// Chapter 09 framebuffer objects'

	glGenFramebuffers( 1, &fbo );
	glBindFramebuffer( GL_FRAMEBUFFER, fbo );

	// Create a texture for out color buffer
	glGenTextures( 1, &color_texture );
	glBindTexture( GL_TEXTURE_2D, color_texture );
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_RGBA8, 512, 512 );

	// turn off mipmaps
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	// Create texture for depth buffer
	glGenTextures( 1, &depth_texture );
	glBindTexture( GL_TEXTURE_2D, depth_texture );
	glTexStorage2D( GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 512, 512 );

	// Attach the color and depth to the fbo
	glFramebufferTexture( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture, 0);
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	// Draw into the buffer
	static const GLenum draw_buffers[] = 
	{
		GL_COLOR_ATTACHMENT0
	};
	glDrawBuffers( 1, draw_buffers );
	
	// once new framebuffer is setup... return to the default
	glBindFramebuffer( GL_FRAMEBUFFER, 0 );

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
	GLuint fontTexture = textureMan.LoadTexture( "font01.tga" );
	
	if( fontTexture != 0 )
		resTexture.AddResource( "SYS_Font01", fontTexture );
	
	// Load the displace Texture (testing)
	GLuint displaceTexture = textureMan.LoadTexture( "displace_test.bmp" );
	
	if( displaceTexture != 0 )
		resTexture.AddResource( "DisplaceTest", displaceTexture );

	// PNG test image
	/*GLuint pngTest = textureMan.LoadTexture( "testPNG.png" );
	
	if( pngTest != 0 )
		resTexture.AddResource( "pngTest", pngTest );*/

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

	GEMesh newMesh( meshType, mesh->getNumVerts(), mesh->getNumIndicies(), newVertexArrayObject, newVertexBuffer, newIndexBuffer, newIndirectBuffer, mesh->getBoundingBox() );
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

void GraphicsEngineOpenGL::SetMouseMode( unsigned char mouseMode )
{
	switch( mouseMode )
	{
	case GE_MOUSEMODE_LOOK:
		glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
		break;
	case GE_MOUSEMODE_POINT:
		glfwSetInputMode( window, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
		break;
	}
}


GEvec2 GraphicsEngineOpenGL::GetMousePosition() const
{
	GEvec2 returnPos;

	double xPos;
	double yPos;

	glfwGetCursorPos( window, &xPos, &yPos );

	returnPos.x = (float)xPos;
	returnPos.y = (float)yPos;

	return returnPos;
}

std::vector<std::string> GraphicsEngineOpenGL::MouseOver( bool findClosest, unsigned char collisionMode )
{
	// list of objects the mouse is over
	std::vector<std::string> objectList;

	// get a reference to the input object

	const GEObject* isObject = gameEntities->GetObject( "SYS_Input_State" );
	const GEObject* gesObject = gameEntities->GetObject( "SYS_GameEngine_Settings" );
	
	if ( isObject != nullptr )
	{
		const InputStateHolder* inputStateHolder = (InputStateHolder*)isObject;
		const GEInputState* inputState = inputStateHolder->getInputState();

		if( inputState->getMouseMode() == GE_MOUSEMODE_POINT )
		{

			const InfoGameEngineSettings* gameEngineSettings = (InfoGameEngineSettings*)gesObject;

			const CameraObject* renderCam = (CameraObject*)gameEntities->GetObject( gameEngineSettings->getRenderCam() );

			if( renderCam != nullptr )
			{
				if (renderCam->getClassName() == "CameraPerspective" )
				{
					// construct a ray from the position of the mouse that shoots into the scene.

					// get the camera view and projection
					glm::mat4 projMatrix = glm::perspective( ((CameraPerspective*)renderCam)->getFovFinal( gameEntities ), (float)gameEngineSettings->getViewportWidth()/(float)gameEngineSettings->getViewportHeight(), 0.1f, 1000.0f);
					glm::mat4 viewMatrix = renderCam->GetViewMatrix( gameEntities );

					// the the mouse position on the screen.
					GEvec2 mousePosition = inputState->getMousePosition();

					// unproject the mouse position twice...

					// first at a near point
					glm::vec4 viewport = glm::vec4(0, 0, gameEngineSettings->getViewportWidth(), gameEngineSettings->getViewportHeight() );
					glm::vec3 winCoord = GEvec3(mousePosition.x,  gameEngineSettings->getViewportHeight() - mousePosition.y - 1, 0.0f);
					glm::vec3 mouseRayPoint01 = glm::unProject(winCoord, viewMatrix, projMatrix, viewport);

					// then at a distant point
					winCoord = GEvec3(mousePosition.x,  gameEngineSettings->getViewportHeight() - mousePosition.y - 1, 1.0f);
					GEvec3 mouseRayPoint02 = glm::unProject(winCoord, viewMatrix, projMatrix, viewport);

					// build the ray out of it.
					GERay mouseRay = GERay( mouseRayPoint01, mouseRayPoint02 - mouseRayPoint01 );


					// go through all the objects... to find which it hits.  TODO optimize so that it doesnt need to go through them all.  Some kind of BSP tree perhaps.
					for ( std::map< std::string, GEObject* >::const_iterator it = gameEntities->First(); it != gameEntities->Last(); it++ )
					{
						// make sure that it can be seen.  mouse can't hit something invisible after all can it?
						if( it->second->isVisible() )
						{
							if( collisionMode == GE_COLLIDE_BOUNDINGBOX )
							{
								// we're going to be doing ray-box collision obviously.
								// we have our ray already... we need to get the box.

								// find what mesh this object uses
								std::string meshName = it->second->getMesh();

								// then we need to get the mesh itself
								GEMesh mesh = resMesh.GetResource( meshName );

								// now get its bounding box.
								GEBoundingBox meshBB = mesh.getBoundingBox();

								// the bounding box is in object space, the ray is in world space.  We need to transform
								// one of them.  Its easier to check for collision if the box is in object space so we'll
								// transform the ray into object space.  We need to construct an inverse matrix of the 
								// objects transforms to do this.
								glm::mat4 rayTransMatrix = glm::inverse( it->second->GetTransformMatrix( gameEntities ) );

								// transform the ray
								GERay tMouseRay = mouseRay * rayTransMatrix;  // these may appear backwards but are multiplied in the correct order in the operator.

								// check for collision
								GEvec3* intersectPoint = meshBB.intersectRay( tMouseRay );

								if( intersectPoint != nullptr )
								{
									objectList.push_back( it->first );  // push back the name of the object.
									delete intersectPoint;
								}
							}

						}
					}
				}
			}
		}

		
	}

	// report if the mouse is over
	if( !objectList.empty() )
	{
		printf( "mouse over\t\r" );
	}
	else
	{
		printf( "mouse not over\r" );
	}

	return objectList;
}
