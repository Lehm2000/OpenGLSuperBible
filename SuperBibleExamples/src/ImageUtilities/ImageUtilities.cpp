/**
	ImageUtilities.cpp
	Purpose: 

	@author Jeff Adams
*/

#include <iostream>
#include <fstream>
#include <cstring>

#include "ImageUtilities.h"
#include "GEImage.h"


GEImage LoadBitmap(const char* filename)
{
	GEImage returnImage;

	std::ifstream inFile;	//input file stream for reading file.
	BitmapHeader header;	//place to hold the header info which is common to all bitmap formats.
	
	
	inFile.open (filename, std::ifstream::in | std::ifstream::binary);

	if (inFile.is_open())
	{
		//read each member from the file.  Cannot read header struct in one go because struct padding can change.
		inFile.read((char*) &header.fileType, sizeof(header.fileType));
		inFile.read((char*) &header.fileSize, sizeof(header.fileSize));
		inFile.read((char*) &header.reserved1, sizeof(header.reserved1));
		inFile.read((char*) &header.reserved2, sizeof(header.reserved2));
		inFile.read((char*) &header.dataOffset, sizeof(header.dataOffset));

		unsigned int dibHeaderSize;

		inFile.read((char*) &dibHeaderSize, sizeof(dibHeaderSize));

		if (dibHeaderSize == 40) // Windows NT & 3.1x or later
		{
			//define place to hold remaining header info
			BITMAPINFOHEADER dibHeader;

			//copy header size into it
			dibHeader.headerSize = dibHeaderSize;

			//read rest of info from file
			inFile.read((char*) &dibHeader.width, sizeof(dibHeader.width));
			inFile.read((char*) &dibHeader.height, sizeof(dibHeader.height));
			inFile.read((char*) &dibHeader.colorPlanes, sizeof(dibHeader.colorPlanes));
			inFile.read((char*) &dibHeader.bitDepth, sizeof(dibHeader.bitDepth));
			inFile.read((char*) &dibHeader.compressionType, sizeof(dibHeader.compressionType));
			inFile.read((char*) &dibHeader.imageSize, sizeof(dibHeader.imageSize));
			inFile.read((char*) &dibHeader.hRes, sizeof(dibHeader.hRes));
			inFile.read((char*) &dibHeader.vRes, sizeof(dibHeader.vRes));
			inFile.read((char*) &dibHeader.numPaletteColors, sizeof(dibHeader.numPaletteColors));
			inFile.read((char*) &dibHeader.numImportantColors, sizeof(dibHeader.numImportantColors));
		
			//read color table info for palettes next
			//TODO 

			//read image data
			//First set read location
			inFile.seekg(header.dataOffset);

			//create place to hold data
			unsigned char* imageFileData = new unsigned char[dibHeader.imageSize];

			//read the image data from the file
			inFile.read((char*) imageFileData, dibHeader.imageSize);

			//now process the data into the GEImage format so it can be used.

			if (dibHeader.compressionType == BI_RGB)  //no compression
			{
				//fill in basic info
				returnImage.setHeight(dibHeader.height);
				returnImage.setWidth(dibHeader.width);
				returnImage.setNumChannels(dibHeader.bitDepth/8);  //TODO: handle bitmap not having bitdepth of 24 or 32
				
				//create space to store the data
				unsigned int dataSize = dibHeader.height * dibHeader.width * returnImage.getNumChannels() * sizeof(float);
				float* tempData = (float*)malloc(dataSize);

				//calculate padded row size (bitmap require multiples of 4 bytes for row data) using integer math. 
				//(basically rounds up to next multiple of 4)
				int paddedFileRowSize = ((dibHeader.bitDepth * dibHeader.width + 31) / 32) * 4;
				
				//also need the row size for the destination which is not padded.
				int unpaddedFileRowSize = returnImage.getWidth() * returnImage.getNumChannels();

				unsigned int i,j,k;

				//iterate through all the rows
				for (i = 0; i < returnImage.getHeight(); i++)
				{ 
					//at what memory location in imageFileData does row begin.
					unsigned int rowStartLocation = paddedFileRowSize * i; 

					//iterate throught the columns in the rows.
					for (j = 0; j < returnImage.getWidth(); j++)
					{
						//iterate through the color channels
						for (k = 0; k < returnImage.getNumChannels(); k++)
						{
							//determine locations to read/write
							unsigned int locDest = (i * unpaddedFileRowSize) + (j * returnImage.getNumChannels()) + k;
							unsigned int locSource = (i * paddedFileRowSize) + (j * returnImage.getNumChannels()) + k;

							//read and convert data to float from source and place in destination.
							tempData[locDest] = (float)imageFileData[locSource] / 255.0f;
						}
					}
					
				}

				//put the loaded and converted image data into the return image.
				returnImage.setData(dataSize, tempData);

				//do some cleanup
				delete[] tempData;
				tempData = nullptr;
			}
			else
			{
				//TODO: error handle unsupported compression type
			}

			inFile.close();
			
			//debug output file info
			printf("\nFile Type: %c%c\n",header.fileType[0],header.fileType[1]);
			printf("File Size: %d\n",header.fileSize);
			printf("%d%d\n",header.reserved1[0],header.reserved1[1]);
			printf("%d%d\n",header.reserved2[0],header.reserved2[1]);
			printf("Data Offset: %d\n",header.dataOffset);
			printf("DIB Header Size: %d\n",dibHeader.headerSize);
			printf("Image Width: %d\n",dibHeader.width);
			printf("Image Height: %d\n",dibHeader.height);
			printf("Color Planes: %d\n",dibHeader.colorPlanes);
			printf("Color Depth: %d\n",dibHeader.bitDepth);
			printf("Compression Type: %d\n",dibHeader.compressionType);
			printf("Image Size: %d\n",dibHeader.imageSize);
			printf("Horizontal Resolution: %d\n",dibHeader.hRes);
			printf("Vertical Resolution: %d\n",dibHeader.vRes);
			printf("Number Palette Colors: %d\n",dibHeader.numPaletteColors);
			printf("Number Important Colors: %d\n",dibHeader.numImportantColors);


			//Clean UP
			delete[] imageFileData;
			imageFileData = nullptr;
			
		}

		

	}
	else
	{
		printf("LoadBitmap: Error opening file");
	}

	return returnImage;
}

GEImage LoadTarga(const char* filename)
{
	GEImage returnImage;

	std::ifstream inFile;		//	input file stream for reading file.
	TargaHeader header;			//	place to hold the header info which is common to all bitmap formats.
	unsigned char* imageID = nullptr;	//	place to hold image ID string.  Varies in size and not always present
	unsigned char* imageData = nullptr;	//	where the image data will be read into.
	
	
	inFile.open (filename, std::ifstream::in | std::ifstream::binary);

	if (inFile.is_open())
	{
		//read each member from the file.  Cannot read header struct in one go because struct padding can change.
		inFile.read((char*) &header.idLen, sizeof(header.idLen));
		inFile.read((char*) &header.colorMapType, sizeof(header.colorMapType));
		inFile.read((char*) &header.imageType, sizeof(header.imageType));
		
		inFile.read((char*) &header.colorMapFirstIndex, sizeof(header.colorMapFirstIndex));
		inFile.read((char*) &header.colorMapLength, sizeof(header.colorMapLength));
		inFile.read((char*) &header.colorMapEntrySize, sizeof(header.colorMapEntrySize));

		inFile.read((char*) &header.xOrigin, sizeof(header.xOrigin));
		inFile.read((char*) &header.yOrigin, sizeof(header.yOrigin));
		inFile.read((char*) &header.width, sizeof(header.width));
		inFile.read((char*) &header.height, sizeof(header.height));
		inFile.read((char*) &header.bitDepth, sizeof(header.bitDepth));
		inFile.read((char*) &header.imageDesc, sizeof(header.imageDesc));

		//	calculate some values
		unsigned char numChannels = header.bitDepth / 8;
		unsigned int imageDataSize = header.width * header.height * ( numChannels );	//	how many bytes should the image take.

		if (header.idLen)  //if idLen is not 0
		{
			//read the id string located after the header.
			imageID = (unsigned char*)malloc(header.idLen);
			inFile.read((char*)imageID, header.idLen);
		}

		//color table info.
		if (header.colorMapType)	//	includes color table.
		{
			//	TODO: deal with color tables
		}

		//	set the basic image info
		returnImage.setHeight(header.height);
		returnImage.setWidth(header.width);
		returnImage.setNumChannels( numChannels );  //TODO: handle bitmap not having bitdepth of 24 or 32

		//	read the image data
		if (header.imageType == TGA_UNCOMPRESSED_RGB)	//	Uncompressed 
		{
			//	allocate memory to hold the image data
			imageData = (unsigned char*)malloc( imageDataSize );

			//read the data
			inFile.read((char*)imageData, imageDataSize);

		}
		else if (header.imageType == TGA_RLE_RGB)	//	Runlength encoded RGB images.
		{
			
			unsigned int readAmount = 0;														//	how many bytes have been read so far.
			
			unsigned char imagePacketHeader;	//	place to hold the 1 byte packet header.
			unsigned char* imagePacketData=nullptr;		//	place to hold the packet data.  Can vary in size.

			//	allocate memory to hold the image data
			imageData = (unsigned char*)malloc( imageDataSize );

			

			while (readAmount < imageDataSize)
			{
				//read the header byte.
				inFile.read((char*) &imagePacketHeader, sizeof(imagePacketHeader));

				if (imagePacketData!=nullptr)
				{
					delete[] imagePacketData;	//	delete existing Packet Data
				}

				//now check what kind of packet it is. check the first bit.
				if (imagePacketHeader & TGA_RLE_PACKET)
				{
					//	Packet is an RLE Packet
					printf("\nRLE Packet Found: Len: %d",(imagePacketHeader-TGA_RLE_PACKET) + 1);

					//	rest of image data in packet will be one one pixel worth. 
					
					imagePacketData = (unsigned char*)malloc(numChannels * sizeof(char));
					inFile.read((char*) imagePacketData, numChannels);
					
					//	we now have have the header packet and the image data.  time to copy it into place.

					for (unsigned char i=0; i < (imagePacketHeader-TGA_RLE_PACKET) + 1; i++)	//The header packet contains the number of times the imagePacketData gets copied.  Remove the first bit (TGA_RLE_PACKET) from the header and add one.
					{
						//TODO: The below code probably only works for images with 24 and 32 bit depths.  Modify to allow other bit depths.
						for (unsigned char j=0; j < numChannels; j++)  //	go through each byte and copy it to the proper place
						{
							imageData[readAmount] = imagePacketData[j];
							readAmount++;
						}
					}

				}
				else
				{
					//	Packet is a RAW Packet
					printf("\nRAW Packet Found:");

					//	 The remainer of the packet header (bits 0-6) contain the size. Since RAW packets have the 7th bit as 0 we can safely use imagePacketHeader as the size (+1).
					unsigned char numImagePixels = imagePacketHeader + 1;

					//allocate enough space for the packet data.    
					imagePacketData = (unsigned char*)malloc( numImagePixels * numChannels );
					inFile.read((char*) imagePacketData, numImagePixels * numChannels );

					//copy packet data into image data.
					memcpy(&imageData[readAmount] ,imagePacketData, numImagePixels * numChannels );
					readAmount += numImagePixels * numChannels ;
				}
			}
		}
		else
		{
			//TODO: Handle unsupported image types.
		}

		//allocate the space for the image in float date
		float* tempData = (float*)malloc(imageDataSize * sizeof(float));

		//now convert the data to GEImage format.
		for (unsigned int i = 0; i < header.width * header.height; i++)	//	iterate through pixels
		{
			for (unsigned int j = 0; j < numChannels ; j++)	//	iterate through channels
			{
				//	set copy source and dest locations
				unsigned int locSource= (i * numChannels )+j;
				unsigned int locDest = (i * numChannels )+j;
				
				//	We must change the order of the channels.  Targa stores it BGRA for some reason.
				if (j == 0)
					locSource+=2;
				else if (j == 2)
					locSource-=2;

				//	copy the data to the tempData buffer.
				tempData[locDest] = (float)imageData[locSource] / 255.0f;
			}
		}
			
		//put the loaded and converted image data into the return image.
		returnImage.setData(imageDataSize * sizeof(float), tempData);
			
		//do some cleanup
		delete[] tempData;
		tempData = nullptr;

		//close the open file.
		inFile.close();

		//debug output file info
		printf("\nID length: %d\n",header.idLen);
		printf("Color map type: %d\n",header.colorMapType);
		printf("Image type: %d\n",header.imageType);
		
		printf("First entry index: %d\n",header.colorMapFirstIndex);
		printf("Color map length: %d\n",header.colorMapLength);
		printf("Color map entry size: %d\n",header.colorMapEntrySize);
		
		
		printf("X-origin: %d\n",header.xOrigin);
		printf("Y-origin: %d\n",header.yOrigin);
		printf("Image width: %d\n",header.width);
		printf("Image height: %d\n",header.height);
		printf("Pixel depth: %d\n",header.bitDepth);
		printf("Image descriptor: %d\n",header.imageDesc);
		
	}
	else
	{
		printf("Error opening file");
	}

	//cleanup
	if (imageID != nullptr)
	{
		delete[] imageID;
		imageID = nullptr;
	}

	if (imageData != nullptr)
	{
		delete[] imageData;
		imageData = nullptr;
	}

	return returnImage;
}
