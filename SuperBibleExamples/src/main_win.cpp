

#include <stdlib.h>
#include <stdio.h>

#include "GameEngine.h"

#include "CameraObject.h"
#include "vmath.h"

int main(void)
{
	bool gameRunning = true;

	CameraObject testCam(glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(1.0f,1.0f,1.0f),true,glm::vec3(1.0f,1.0f,1.0f));

	glm::vec3 testRot = testCam.getRotation();

	printf("%f %f\n",testRot[0], testRot[1]);
	
	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();
	gameEngine.CreateGameCam(CAMTYPE_PERSPECTIVE,glm::vec3( 0.0f, 0.0f, 0.0f ), glm::vec3( 0.0f, 0.0f, 0.0f ), 45.0f );

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