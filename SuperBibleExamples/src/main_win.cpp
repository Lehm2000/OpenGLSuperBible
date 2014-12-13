

#include <stdlib.h>
#include <stdio.h>
#include <glm\glm.hpp>

#include "GEObject.h"
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
#include "MeshUtilities.h"
#include "InfoGameEngineSettings.h"
#include "Orbiter.h"
#include "BitReader.h"
#include "GERay.h"
#include "GEPlane.h"


int main(void)
{
	
	bool gameRunning = true;

	// testing stuff goes here

	// Plane-Ray intersection

	GERay ray = GERay( GEvec3( 0.0f, 0.0f, 5.0f ), GEvec3( 1.0f, 0.0f, -10.0f ) );
	//GEPlane plane = GEPlane( GEvec3( -10.0f, 0.0f, -3.0f ), GEvec3( 0.0f, 10.0f, -3.0f ), GEvec3( 0.0f, 0.0f, -3.0f ) );
	GEBoundingBox box = GEBoundingBox( GEvec3(-3.0f, -3.0f, -3.0f), GEvec3(3.0f, 3.0f, 3.0f) ); 

	GEvec3* rvIntersect = box.intersectRay( ray );
	
	// initialize the game engine.

	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();
	
	// create the main render camera

	//gameEngine.CreateGameCam(CAMTYPE_PERSPECTIVE,GEvec3( 0.0f, 0.0f, 2.5f ), GEvec3(0.0f, 0.0f, 0.0f ), glm::radians(45.0f) );
	CameraPerspective* camObject = new CameraPerspective( GEvec3( 0.0f, 0.0f, 3.0f ), GEvec3(0.0f, 0.0f, 0.0f ), glm::radians(45.0f) );
	camObject->addRotationController( new GEControllerInputMousePositionXv3( GEvec3( 0.0f, -0.0025f, 0.0f ) ) );
	camObject->addRotationController( new GEControllerInputMousePositionYv3( GEvec3( -0.0025f, 0.0f, 0.0f ) ) );
	/*camObject->setRotationMax( GEvec3( 0.5f, 0.5f, 0.05f ) );
	camObject->setRotationUseMax( true );
	camObject->setRotationMin( GEvec3( -0.5f, -0.5f, 0.05f ) );
	camObject->setRotationUseMin( true );*/
	camObject->addFovController( new GEControllerInputMouseScrollYf1( -0.10f ) );
	gameEngine.AddEntity( "gameCam01", camObject );

	// tell the game to use it.

	InfoGameEngineSettings* gameEngineSettings = (InfoGameEngineSettings*)gameEngine.GetEntity( "SYS_GameEngine_Settings" );

	if( gameEngineSettings != nullptr )
	{
		gameEngineSettings->setRenderCam( "gameCam01" );
	}

	// Create a test object


	GEObject* testObject = new GEObject( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( .5f, .5f, .5f ) );
	testObject->setMesh( "testTube" );
	
	std::vector< std::string> defaultMatList;
	defaultMatList.push_back ( "default" );
	
	testObject->setMaterialValueList( defaultMatList );
	testObject->setMaterialValue( "default" );

	testObject->addRotationController( new GEControllerConstantv3( GEvec3( 1.0f, 0.0f, 0.0f ) ) );
	testObject->addPositionController( new GEControllerOscillatorv3( GEvec3( 0.0f, 1.0f, 0.0f), 6.0f ) );
	testObject->addPositionController( new GEControllerOscillatorv3( GEvec3( 2.0f, 0.0f, 1.0f), 7.0f ) );
	gameEngine.AddEntity( "testObject", testObject );

	Orbiter* testOrbiter = new Orbiter( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( .25f, .25f, .25f ),
		"testObject", GEvec3( 0.0f, 0.0f, 0.0f ), 1.0f );
	testOrbiter->setMesh( "cube" );
	
	testOrbiter->setMaterialValueList( defaultMatList );
	testOrbiter->setMaterialValue( "default" );
	
	testOrbiter->addOrbitAngleController( new GEControllerConstantv3( GEvec3( 0.0f, 2.0f, 0.0f ) ) );
	gameEngine.AddEntity( "testOrbiter", testOrbiter );

	Orbiter* testOrbiter2 = new Orbiter( GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( .25f, .25f, .25f ),
		"testOrbiter", GEvec3( 0.0f, 0.0f, 0.0f ), 0.5f );
	testOrbiter2->setMesh( "cube" );
	
	testOrbiter2->setMaterialValueList( defaultMatList );
	testOrbiter2->setMaterialValue( "default" );
	
	testOrbiter2->addOrbitAngleController( new GEControllerConstantv3( GEvec3( 0.0f, -2.0f, 0.0f ) ) );
	gameEngine.AddEntity( "testOrbiter2", testOrbiter2 );

	GEObject* testObject2 = new GEObject( GEvec3( 1.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 1.0f, 1.0f, 1.0f ) );
	testObject2->setMesh( "cube" );

	testObject2->setMaterialValueList( defaultMatList );
	testObject2->setMaterialValue( "default" );

	//testObject2->addRotationController( new GEControllerInputMousePosition( GEvec3( 0.01f, 0.01f, 0.0f ) ) );
	//testObject2->getRotation()->addController( new GEControllerConstantv3( GEvec3( 1.0f, 0.0f, 0.0f ) ), testObject2 );
	testObject2->addPositionController( new GEControllerInputKeyv3( GEvec3( 1.0f, 0.0f, 0.0f ), GE_KEY_D) );
	testObject2->addPositionController( new GEControllerInputKeyv3( GEvec3( -1.0f, 0.0f, 0.0f ), GE_KEY_A) );
	testObject2->addScaleController( new GEControllerOscillatorv3( GEvec3( 0.2f, -0.5f, 0.5f ) , 3.0f ) );
	
	gameEngine.AddEntity( "testObject2", testObject2 );

	//====================================================================================================================
	GEObject* testObject3 = new GEObject( GEvec3( -1.0f, 0.0f, 0.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( .25f, .25f, .25f ) );
	testObject3->setMesh( "plane" );

	std::vector< std::string> testObject3MatList;
	testObject3MatList.push_back( "texture_test" );

	testObject3->setMaterialValueList( testObject3MatList );
	testObject3->setMaterialValue( "texture_test" );
	
	testObject3->addRotationController( new GEControllerLookAt( "testObject") );
	gameEngine.AddEntity( "testObject3", testObject3 );


	//back object ====================================================================================
	GEObject* testObject4 = new GEObject( GEvec3( 0.0f, -0.0f, 6.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 1.0f, 1.0f, 1.0f ) );
	testObject4->setMesh( "cube" );
	
	testObject4->setMaterialValueList( defaultMatList );
	testObject4->setMaterialValue( "default" );
	
	gameEngine.AddEntity( "testObject4", testObject4 );

	// right object ======================================================================================================
	GEObject* testObject5 = new GEObject( GEvec3( 6.0f, -0.0f, 3.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 1.0f, 1.0f, 1.0f ) );
	testObject5->setMesh( "cube" );
	
	testObject5->setMaterialValueList( defaultMatList );
	testObject5->setMaterialValue( "default" );
	
	gameEngine.AddEntity( "testObject5", testObject5 );

	// left object ====================================================================================================
	GEObject* testObject6 = new GEObject( GEvec3( -6.0f, -0.0f, 3.0f ), GEvec3( 0.0f, 0.0f, 0.0f ), GEvec3( 1.0f, 1.0f, 1.0f ) );
	testObject6->setMesh( "cube" );
	
	testObject6->setMaterialValueList( defaultMatList );
	testObject6->setMaterialValue( "default" );
	
	gameEngine.AddEntity( "testObject6", testObject6 );
	
	

	while (gameRunning)
	{

		//Eventually this loop will be in the game class.

		gameEngine.Update();
		gameEngine.Render();

		gameRunning = gameEngine.isRunning();

	}

	exit(EXIT_SUCCESS);

}