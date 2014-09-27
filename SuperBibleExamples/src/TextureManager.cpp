
/**
	TextureManager.cpp
	Purpose: Handles all Texture Functions.

	@author Jeff Adams
*/


#include "TextureManager.h"


#include "ImageUtilities\IUImage.h"

/**
	Default Constructor
*/
TextureManager::TextureManager()
{
	//Define the relative path to the texture directory. 
	texturePath = "./textures/";
}



GLuint TextureManager::LoadTexture( std::string filename )
{
	GLuint loadedTexture = 0;
	
	ImageUtilities imageUtilities;

	// first determine what kind of texture it is.  For now we'll use the 
	// extension.  May need more robust method later.

	if ( filename != "" )
	{
		unsigned int lastDot = filename.find_last_of(".");
		unsigned int texType = 0;

		if ( lastDot != std::string::npos )
		{
			std::string ext = filename.substr( lastDot + 1, filename.length() - lastDot );

			std::string fullFileName = texturePath;
			fullFileName.append( filename );

			if( ext == "bmp" )
			{
				IUImage<unsigned char> image = imageUtilities.LoadBitmap( fullFileName.c_str() );

				unsigned char* imageTexData = nullptr;

				imageTexData = (unsigned char*)malloc( image.getDataSize() );
				image.getData( imageTexData );

				glGenTextures(1, &loadedTexture);
				glBindTexture(GL_TEXTURE_2D, loadedTexture);

				glTexStorage2D( GL_TEXTURE_2D, 5, GL_RGB8, image.getWidth(), image.getHeight() );
				glTexSubImage2D( GL_TEXTURE_2D,0,0,0, image.getWidth(), image.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, imageTexData );

				glGenerateMipmap( GL_TEXTURE_2D );

				delete[] imageTexData;
				imageTexData = nullptr;
			}
		}

		/*
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
		}*/
	}

	return loadedTexture;
}


