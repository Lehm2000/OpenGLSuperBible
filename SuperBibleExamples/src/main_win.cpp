

#include <stdlib.h>
#include <stdio.h>

#include "GameEngine.h"

#include "CameraObject.h"
#include "CameraPerspective.h"
#include "GEControllerConstant.h"
#include "GEControllerOscillator.h"
#include "GEControllerLookAt.h"

int main(void)
{
	bool gameRunning = true;
	
	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();
	gameEngine.CreateGameCam(CAMTYPE_PERSPECTIVE,glm::vec3( 0.0f, 0.0f, 2.5f ), glm::vec3(0.0f, 0.0f, 0.0f ), glm::radians(45.0f) );

	// Create a test object

	GEController* tempPosCon;
	GEController* tempRotCon;
	GEController* tempScaCon;

	GEObject* testObject = new GEObject( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( .5f, .5f, .5f ), "test_object");
	testObject->setMesh( "sphere" );
	testObject->setMaterial( "default" );
	testObject->addRotationController ( new GEControllerConstant( glm::vec3( 1.0f, 1.0f, 0.0f ) ) );
	testObject->addPositionController ( new GEControllerOscillator( glm::vec3( 0.0f, 1.0f, 0.0f), 6.0f ) );
	testObject->addPositionController ( new GEControllerOscillator( glm::vec3( 2.0f, 0.0f, 1.0f), 7.0f ) );
	gameEngine.AddEntity( "testObject", testObject );

	GEObject* testObject2 = new GEObject( glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ), "test_object2");
	testObject2->setMesh( "cube" );
	testObject2->setMaterial( "default" );
	testObject2->addScaleController( new GEControllerOscillator( glm::vec3( 0.25f, 0.25f, 0.25f ) , 3.0f ) );
	
	gameEngine.AddEntity( "testObject2", testObject2 );

	GEObject* testObject3 = new GEObject( glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( .25f, .25f, .25f ), "test_object3");
	testObject3->setMesh( "diamond" );
	testObject3->setMaterial( "default" );
	//testObject3->addPositionController( new GEControllerOscillator( glm::vec3( 0.5f, 0.5f, 0.5f), 5.0f ) );
	testObject3->addRotationController( new GEControllerLookAt( "testObject") );
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