
/**
	TextureManager.cpp
	Purpose: Handles all Texture Functions.

	@author Jeff Adams
*/


#include "TextureManager.h"

#include "GEImage.h"
#include "ImageUtilities\IUImage.h"

/**
	Default Constructor
*/
TextureManager::TextureManager()
{
	//Define the relative path to the texture directory. 
	texturePath = "./textures/";
}

GEImage TextureManager::LoadTexture( const char* filename, const char texType )
{
	IUImage loadedTexture;
	GEImage finalTexture;

	if ( filename != nullptr && texType != 0 )
	{

		char fullFilename[1024];	//TODO: deal with magic number here.
	
		//combine filepath for shaders with supplied filename.
		strcpy( fullFilename, texturePath.c_str() );
		strcat( fullFilename, filename );

		switch (texType)
		{
		case GE_TEXTYPE_BMP:
			loadedTexture = LoadBitmap( fullFilename );
			break;
		case GE_TEXTYPE_TARGA:
			break;
		default:
			break;
		}

		//check if we successfully loaded an image
		if ( loadedTexture.getDataSize() > 0 )  //Good test?
		{
			//convert it to GEImage format.
			finalTexture.setWidth( loadedTexture.getWidth() );
			finalTexture.setHeight( loadedTexture.getHeight() );
			finalTexture.setNumChannels( loadedTexture.getNumChannels() );

			// allocate a buffer for the converted data
			float* finalTextureDataBuffer = nullptr;
			unsigned int finalTextureDataBufferSize = loadedTexture.getWidth() * loadedTexture.getHeight() * loadedTexture.getNumChannels() * sizeof(float);
			finalTextureDataBuffer = (float*)malloc( finalTextureDataBufferSize );

			// allocate a buffer for the loaded data
			unsigned char* loadedTextureDataBuffer = nullptr;
			loadedTextureDataBuffer = (unsigned char*)malloc( loadedTexture.getDataSize() );

			// copy the loaded data into the buffer.
			loadedTexture.getData( loadedTextureDataBuffer );

			unsigned int temp1 = loadedTexture.getWidth() * loadedTexture.getHeight() * loadedTexture.getNumChannels() * sizeof(float);
			unsigned int temp2 = loadedTexture.getDataSize();

			for (int i = 0; i < loadedTexture.getWidth() * loadedTexture.getHeight() * loadedTexture.getNumChannels() ; i++)
			{
				//printf("%d ",i);
				finalTextureDataBuffer[i] = loadedTextureDataBuffer[i] / 255.0f;
			}

			finalTexture.setData( finalTextureDataBufferSize, finalTextureDataBuffer );

			//do some cleanup
			delete[] finalTextureDataBuffer;
			finalTextureDataBuffer = nullptr;

			delete[] loadedTextureDataBuffer;
			loadedTextureDataBuffer = nullptr;
		}

		
	}

	return finalTexture;
}

