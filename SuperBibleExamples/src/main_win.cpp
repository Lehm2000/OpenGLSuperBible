

#include <stdlib.h>
#include <stdio.h>

#include "GraphicsEngine.h"

int main(void)
{
	
	GraphicsEngine myGraphics; //make pointer?... this will go inside the game engine eventually.

	while(!myGraphics.CheckWindowClose())
	{

		//Eventually this will call the game engine which will then call render

		myGraphics.Render( myGraphics.getCurrentTime() );

	}

	exit(EXIT_SUCCESS);

}