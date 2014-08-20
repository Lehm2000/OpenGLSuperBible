
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
		if ( loadedTexture.getDataSize() > 0 )  //Sufficient test?
		{
			//convert it to GEImage format.
			finalTexture = ConvertTexture(&loadedTexture);
			
		}

		
	}

	return finalTexture;
}

GEImage TextureManager::ConvertTexture(const IUImage* sourceTexture)
{
	GEImage outputTexture;

	outputTexture.setWidth( sourceTexture->getWidth() );
	outputTexture.setHeight( sourceTexture->getHeight() );
	outputTexture.setNumChannels( sourceTexture->getNumChannels() );

	// allocate a buffer for the converted data
	float* outputTextureDataBuffer = nullptr;
	unsigned int outputTextureDataBufferSize = sourceTexture->getWidth() * sourceTexture->getHeight() * sourceTexture->getNumChannels() * sizeof(float);
	outputTextureDataBuffer = (float*)malloc( outputTextureDataBufferSize );

	// allocate a buffer for the loaded data
	unsigned char* loadedTextureDataBuffer = nullptr;
	loadedTextureDataBuffer = (unsigned char*)malloc( sourceTexture->getDataSize() );

	// copy the loaded data into the buffer.
	sourceTexture->getData( loadedTextureDataBuffer );

	for (unsigned int i = 0; i < sourceTexture->getWidth() * sourceTexture->getHeight() * sourceTexture->getNumChannels() ; i++)
	{
		//printf("%d ",i);
		outputTextureDataBuffer[i] = loadedTextureDataBuffer[i] / 255.0f;
	}

	outputTexture.setData( outputTextureDataBufferSize, outputTextureDataBuffer );

	//do some cleanup
	delete[] outputTextureDataBuffer;
	outputTextureDataBuffer = nullptr;

	delete[] loadedTextureDataBuffer;
	loadedTextureDataBuffer = nullptr;

	return outputTexture;
}

