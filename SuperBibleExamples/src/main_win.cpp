

#include <stdlib.h>
#include <stdio.h>

#include "GraphicsEngine.h"


int main(void)
{
	
	GraphicsEngine myGraphics; //make pointer?

	
	while(!myGraphics.CheckWindowClose())
	{
		myGraphics.Render( myGraphics.getCurrentTime() );

	}

	
	exit(EXIT_SUCCESS);

}