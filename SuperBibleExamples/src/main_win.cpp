

#include <stdlib.h>
#include <stdio.h>

#include "GameEngine.h"

int main(void)
{
	bool gameRunning = true;
	
	GameEngine gameEngine; //make pointer?... this will go inside the game class eventually

	gameEngine.Initialize();

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