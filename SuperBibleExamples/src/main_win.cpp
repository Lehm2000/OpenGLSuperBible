

#include <stdlib.h>
#include <stdio.h>

#include "GameEngine.h"

#include "CameraObject.h"
#include "PerspectiveCamera.h"
#include "GEControllerConstant.h"
#include "GEControllerOscillator.h"

int main(void)
{
	bool gameRunning = true;

	CameraObject* testCam= new PerspectiveCamera(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), 45.0f, true, glm::vec3(1.0f,1.0f,1.0f));

	printf( "%s\n",testCam->getClassName().c_str() );

	//glm::vec3 testRot = testCam.getRotation();

	//printf("%f %f\n",testRot[0], testRot[1]);
	
	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();
	gameEngine.CreateGameCam(CAMTYPE_PERSPECTIVE,glm::vec3( 0.0f, 0.0f, 2.5f ), glm::vec3(0.0f, 0.0f, 0.0f ), glm::radians(45.0f) );

	// Create a test object

	GEObject* testObject = new GEObject( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ), "test_object");
	testObject->setMesh( "default" );
	testObject->setMaterial( "default" );
	gameEngine.AddEntity( "testObject", testObject );

	GEObject* testObject2 = new GEObject( glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 1.0f, 1.0f, 1.0f ), "test_object2");
	testObject2->setMesh( "default" );
	testObject2->setMaterial( "default" );
	gameEngine.AddEntity( "testObject2", testObject2 );

	GEObject* testObject3 = new GEObject( glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( .5f, .5f, .5f ), "test_object3");
	testObject3->setMesh( "default" );
	testObject3->setMaterial( "default" );
	GEController* tempPosCon = new GEControllerOscillator( glm::vec3( 0.5f, 0.5f, 0.5f), 5.0f );
	GEController* tempRotCon = new GEControllerConstant( glm::vec3( 1.0f, 1.0f, 1.0f) );
	testObject3->setPositionController ( tempPosCon );
	testObject3->setRotationController ( tempRotCon );
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