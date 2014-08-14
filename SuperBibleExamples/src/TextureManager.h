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

class TextureManager
{
private:
	
	std::string texturePath; //the path to the textures.

public:

	//constructors
	TextureManager();

	//Functions
	void LoadBitmap(const char* filename);
	
};

#endif /* TEXTUREMANAGER_H */
