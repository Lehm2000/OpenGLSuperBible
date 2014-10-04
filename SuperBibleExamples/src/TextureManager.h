#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glext.h"
#include "ImageUtilities\ImageUtilities.h"
#include "ImageUtilities\IUImage.h"


//Some constants
#define GE_TEXTYPE_BMP 1
#define GE_TEXTYPE_TARGA 2

class TextureManager
{
private:
	
	std::string texturePath; //the path to the textures.

public:

	//constructors
	
	TextureManager();

	//Functions
	
	GLuint LoadTexture( std::string filename );
	
	
};


#endif /* TEXTUREMANAGER_H */
