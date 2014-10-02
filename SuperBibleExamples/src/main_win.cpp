

#include <stdlib.h>
#include <stdio.h>
#include <glm\glm.hpp>

#include "GameEngine.h"

#include "CameraObject.h"
#include "CameraPerspective.h"
#include "GEControllerConstant.h"
#include "GEControllerOscillator.h"
#include "GEControllerLookAt.h"
#include "GEControllerInputKey.h"
#include "GEControllerInputMousePositionX.h"
#include "GEControllerInputMousePositionY.h"
#include "GEControllerInputMouseScrollY.h"
#include "InfoViewport.h"
#include "MeshUtilities.h"


int main(void)
{
	
	bool gameRunning = true;

	// testing mesh stuff here

	MeshUtilities meshUtil;

	meshUtil.LoadASE( "C:\\Programming\\OpenGLSuperBible\\SuperBibleExamples\\meshes\\testBox.ASE" );
	
	
	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();
	
	// create the main render camera

	//gameEngine.CreateGameCam(CAMTYPE_PERSPECTIVE,GEvec3( 0.0f, 0.0f, 2.5f ), GEvec3(0.0f, 0.0f, 0.0f ), glm::radians(45.0f) );
	CameraPerspective* camObject = new CameraPerspective( GEvec3( 0.0f, 0.0f, 2.5f ), GEvec3(0.0f, 0.0f, 0.0f ), glm::radians(45.0f) );
	camObject->getRotation()->addController( new GEControllerInputMousePositionXv3( GEvec3( 0.0f, -0.0025f, 0.0f ) ), camObject );
	camObject->getRotation()->addController( new GEControllerInputMousePositionYv3( GEvec3( -0.0025f, 0.0f, 0.0f ) ), camObject );
	camObject->getRotation()->setMax( GEvec3( 0.5f, 0.5f, 0.05f ) );
	camObject->getRotation()->setUseMax( true );
	camObject->getRotation()->setMin( GEvec3( -0.5f, -0.5f, 0.05f ) );
	camObject->getRotation()->setUseMin( true );
	camObject->getFOV()->addController( new GEControllerInputMouseScrollYf1( -0.10f ), camObject );
	gameEngine.AddEntity( "gameCam01", camObject );

	// tell the game to use it.

	InfoViewport* viewportOptions = (InfoViewport*)gameEngine.GetEntity( "SYS_Viewport_Options" );

	if( viewportOptions != nullptr )
	{
		viewportOptions->setRenderCam( "gameCam01" );
	}

	// Create a test object


	GEObject* testObject = new GEObject( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( .5f, .5f, .5f ), "test_object");
	testObject->setMesh( "testTube" );
	testObject->setMaterial( "default" );
	testObject->getRotation()->addController( new GEControllerConstantv3( GEvec3( 1.0f, 0.0f, 0.0f ) ), testObject );
	testObject->getPosition()->addController( new GEControllerOscillatorv3( GEvec3( 0.0f, 1.0f, 0.0f), 6.0f ), testObject );
	testObject->getPosition()->addController( new GEControllerOscillatorv3( GEvec3( 2.0f, 0.0f, 1.0f), 7.0f ), testObject );
	gameEngine.AddEntity( "testObject", testObject );

	GEObject* testObject2 = new GEObject( GEvec3( 1.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 1.0f, 1.0f, 1.0f ), "test_object2");
	testObject2->setMesh( "cube" );
	testObject2->setMaterial( "default" );
	//testObject2->addRotationController( new GEControllerInputMousePosition( GEvec3( 0.01f, 0.01f, 0.0f ) ) );
	//testObject2->getRotation()->addController( new GEControllerConstantv3( GEvec3( 1.0f, 0.0f, 0.0f ) ), testObject2 );
	testObject2->getScale()->addController( new GEControllerOscillatorv3( GEvec3( 0.2f, -0.5f, 0.5f ) , 3.0f ), testObject2 );
	
	gameEngine.AddEntity( "testObject2", testObject2 );

	GEObject* testObject3 = new GEObject( GEvec3( -1.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( .25f, .25f, .25f ), "test_object3");
	testObject3->setMesh( "plane" );
	testObject3->setMaterial( "texture_test" );
	//testObject3->addPositionController( new GEControllerOscillator( GEvec3( 0.5f, 0.5f, 0.5f), 5.0f ) );
	testObject3->getPosition()->addController( new GEControllerInputKeyv3( GEvec3( 0.0f, 1.0f, 0.0f ), GE_KEY_W ), testObject3 );
	testObject3->getPosition()->addController( new GEControllerInputKeyv3( GEvec3( 0.0f, -1.0f, 0.0f ), GE_KEY_S ), testObject3 );
	testObject3->getPosition()->addController( new GEControllerInputKeyv3( GEvec3( -1.0f, 0.0f, 0.0f ), GE_KEY_A ), testObject3 );
	testObject3->getPosition()->addController( new GEControllerInputKeyv3( GEvec3( 1.0f, -0.0f, 0.0f ), GE_KEY_D ), testObject3 );
	testObject3->getRotation()->addController( new GEControllerLookAt( "testObject"), testObject3 );
	gameEngine.AddEntity( "testObject3", testObject3 );

	while (gameRunning)
	{

		//Eventually this loop will be in the game class.

		gameEngine.Update();
		gameEngine.Render();

		gameRunning = gameEngine.isRunning();

	}

	exit(EXIT_SUCCESS);

}