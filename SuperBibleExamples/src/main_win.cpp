

#include <stdlib.h>
#include <stdio.h>

#include "GameEngine.h"

#include "CameraObject.h"
#include "PerspectiveCamera.h"
#include "vmath.h"

int main(void)
{
	bool gameRunning = true;

	CameraObject* testCam= new PerspectiveCamera(glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,0.0f,0.0f), 45.0f, true, glm::vec3(1.0f,1.0f,1.0f));

	printf( "%s\n",testCam->getClassName().c_str() );

	//glm::vec3 testRot = testCam.getRotation();

	//printf("%f %f\n",testRot[0], testRot[1]);
	
	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();
	gameEngine.CreateGameCam(CAMTYPE_PERSPECTIVE,glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec3(0.0f, 0.0f, 0.0f ), 45.0f );

	// Create a test object

	GEObject* testObject = new GEObject( glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), "test_object");
	testObject->setMesh( "default" );
	testObject->setMaterial( "default" );
	gameEngine.AddEntity( "testObject", testObject );

	//while(!myGraphics.CheckWindowClose())
	while (gameRunning)
	{

		//Eventually this loop will be in the game class.

		//myGraphics.Render( myGraphics.getCurrentTime() );
		gameEngine.Update();
		gameEngine.Render();


		gameRunning = gameEngine.isRunning();

	}

	exit(EXIT_SUCCESS);

}