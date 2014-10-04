
/**
	TextureManager.cpp
	Purpose: Handles all Texture Functions.

	@author Jeff Adams
*/


#include "TextureManager.h"
#include "GEConstants.h"


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

	if ( filename == GE_MISSING )
	{
		// create the 'missing' texture.  Texture that is returned if the specified one is not found.
		// it will be completely purple.
		unsigned char* imageTexData = (unsigned char*)malloc( 64 * 64 * 3 );

		for ( unsigned int i = 0; i < 64 * 64; i++ )
		{
			imageTexData[ i * 3 ] = 255;
			imageTexData[ ( i * 3 ) + 1] = 0;
			imageTexData[ ( i * 3 ) + 2] = 255;
			
		}

		glGenTextures(1, &loadedTexture);
		glBindTexture(GL_TEXTURE_2D, loadedTexture);

		glTexStorage2D( GL_TEXTURE_2D, 5, GL_RGB8, 64, 64);
		glTexSubImage2D( GL_TEXTURE_2D,0,0,0, 64, 64, GL_RGB, GL_UNSIGNED_BYTE, imageTexData );

		glGenerateMipmap( GL_TEXTURE_2D );

		free( imageTexData );  // allocated with malloc
		imageTexData = nullptr;

	}
	else if ( filename != "" )
	{
		unsigned int lastDot = filename.find_last_of(".");
		unsigned int texType = 0;

		if ( lastDot != std::string::npos )
		{
			std::string ext = filename.substr( lastDot + 1, filename.length() - lastDot );

			std::string fullFileName = texturePath;
			fullFileName.append( filename );



			if( ext == "bmp" )  // bitmap
			{
				IUImage<unsigned char> image = imageUtilities.LoadBitmap( fullFileName.c_str() );

				// verify that the texture was loaded

				if( image.getDataSize() > 0 )  // is this the best check?
				{
					unsigned char* imageTexData = nullptr;

					imageTexData = (unsigned char*)malloc( image.getDataSize() );
					image.getData( imageTexData );

					glGenTextures(1, &loadedTexture);
					glBindTexture(GL_TEXTURE_2D, loadedTexture);

					glTexStorage2D( GL_TEXTURE_2D, 5, GL_RGB8, image.getWidth(), image.getHeight() );
					glTexSubImage2D( GL_TEXTURE_2D,0,0,0, image.getWidth(), image.getHeight(), GL_RGB, GL_UNSIGNED_BYTE, imageTexData );

					glGenerateMipmap( GL_TEXTURE_2D );

					free( imageTexData );	// allocated with malloc
					imageTexData = nullptr;
				}
			}
		}

	}

	return loadedTexture;
}


