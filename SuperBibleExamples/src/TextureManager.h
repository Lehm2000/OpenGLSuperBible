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
	
	template <class T>
	IUImage<T> LoadTexture( const char* filename, const char texType, T type );
	//GEImage ConvertTexture(const IUImage<unsigned char>* sourceTexture);
	
};

template <class T>
IUImage<T> TextureManager::LoadTexture( const char* filename, const char texType, T type )
{
	IUImage<T> loadedTexture;
	//GEImage finalTexture;

	ImageUtilities imageUtilities;

	if ( filename != nullptr && texType != 0 )
	{

		char fullFilename[1024];	//TODO: deal with magic number here.
	
		//combine filepath for shaders with supplied filename.
		strcpy( fullFilename, texturePath.c_str() );
		strcat( fullFilename, filename );

		switch (texType)
		{
		case GE_TEXTYPE_BMP:
			loadedTexture = imageUtilities.LoadBitmap( fullFilename );
			break;
		case GE_TEXTYPE_TARGA:
			break;
		default:
			break;
		}

		//check if we successfully loaded an image
		/*
		if ( loadedTexture.getDataSize() > 0 )  //Sufficient test?
		{
			//convert it to GEImage format.
			finalTexture = ConvertTexture(&loadedTexture);
			
		}
		*/

		
	}

	return loadedTexture;
}

#endif /* TEXTUREMANAGER_H */
