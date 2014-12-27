/**
	ImageUtilities.cpp
	Purpose: 

	@author Jeff Adams
*/

#include <iostream>
#include <fstream>
#include <cstring>

#include "ImageUtilities.h"
//#include "..\GEImage.h"
#include "IUImage.h"
#include "../BitReader.h"


IUImage<unsigned char> ImageUtilities::LoadBitmap(const char* filename)
{
	IUImage<unsigned char> returnImage;

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
				//returnImage.setHeight(dibHeader.height);
				//returnImage.setWidth(dibHeader.width);
				//returnImage.setNumChannels(dibHeader.bitDepth/8);  //TODO: handle bitmap not having bitdepth of 24 or 32
				
				//create space to store the data
				//unsigned int dataSize = dibHeader.height * dibHeader.width * returnImage.getNumChannels() * sizeof(float);
				unsigned int dataSize = dibHeader.width * dibHeader.height * (dibHeader.bitDepth/8) * sizeof(unsigned char);
				unsigned char* tempData = (unsigned char*)malloc(dataSize);

				//calculate padded row size (bitmap require multiples of 4 bytes for row data) using integer math. 
				//(basically rounds up to next multiple of 4)
				int paddedFileRowSize = ((dibHeader.bitDepth * dibHeader.width + 31) / 32) * 4;
				
				// also need the row size for the destination which is not padded.
				int unpaddedFileRowSize = dibHeader.width * (dibHeader.bitDepth/8);

				unsigned int totalImageElements = dibHeader.width * dibHeader.height * (dibHeader.bitDepth/8);

				unsigned int i, j, k;

				// iterate through all the rows
				for (i = 0; i < dibHeader.height; i++)
				{ 
					// at what memory location in imageFileData does row begin.
					unsigned int rowStartLocation = paddedFileRowSize * i; 

					// iterate throught the columns in the rows.
					for (j = 0; j < dibHeader.width; j++)
					{
						// iterate through the color channels
						for (k = 0; k < dibHeader.bitDepth/8; k++)
						{
							//determine locations to read/write
							
							// BMP's rows are reversed.. so top row is at the bottom.  So we need to flip the rows.  Thats what the dibHeader.height - 1 is all about.
							unsigned int locDest =  ( ( ( dibHeader.height - 1) - i ) * unpaddedFileRowSize) + (j * (dibHeader.bitDepth/8) ) + k ;
							
							// No need to reverse the source as well.
							unsigned int locSource = (i * paddedFileRowSize) + (j * (dibHeader.bitDepth/8) ) + k;

							// read and place in destination.
							tempData[locDest] = imageFileData[locSource];
						}
					}
					
				}

				// put the loaded and converted image data into the return image... reminder windows bitmaps are BGR
				returnImage.setData( IUI_FORMAT_BGR, dibHeader.width, dibHeader.height, dibHeader.bitDepth/8 , tempData);

				//do some cleanup
				free (tempData );  // allocated with malloc
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
			delete[] imageFileData; // allocated with new.
			imageFileData = nullptr;
			
		}

		

	}
	else
	{
		printf("LoadBitmap: Error opening file");
	}

	return returnImage;
}

IUImage<unsigned char> ImageUtilities::LoadTarga(const char* filename)
{
	IUImage<unsigned char> returnImage;

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
		unsigned int dataSize = header.width * header.height * (numChannels) * sizeof(unsigned char);
		//unsigned int imageDataSize = header.width * header.height * ( numChannels );	//	how many bytes should the image take.

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
		// returnImage.setHeight(header.height);
		// returnImage.setWidth(header.width);
		// returnImage.setNumChannels( numChannels );  //TODO: handle bitmap not having bitdepth of 24 or 32

		//	read the image data
		if (header.imageType == TGA_UNCOMPRESSED_RGB)	//	Uncompressed 
		{
			//	allocate memory to hold the image data
			
			imageData = (unsigned char*)malloc( dataSize );

			//read the data
			inFile.read( (char*)imageData, dataSize );

		}
		else if (header.imageType == TGA_RLE_RGB)	//	Runlength encoded RGB images.
		{
			
			unsigned int readAmount = 0;			//	how many bytes have been read so far.
			
			unsigned char imagePacketHeader;	//	place to hold the 1 byte packet header.
			unsigned char* imagePacketData=nullptr;		//	place to hold the packet data.  Can vary in size.

			//	allocate memory to hold the image data
			
			imageData = (unsigned char*)malloc( dataSize );

			

			while (readAmount < dataSize )
			{
				//read the header byte.
				inFile.read((char*) &imagePacketHeader, sizeof(imagePacketHeader));

				if (imagePacketData!=nullptr)
				{
					free( imagePacketData );	//	delete existing Packet Data... allocated with malloc
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

		//allocate a new buffer for the final image data to go.
		unsigned char* tempData = (unsigned char*)malloc( dataSize );

		//now convert the data to GEImage format and reorder the pixels if necessary.
		for (unsigned int i = 0; i < (unsigned int)header.width; i++)	//	iterate through pixels
		{
			for( unsigned int j = 0; j < (unsigned int)header.height; j++ )
			{
				for (unsigned int k = 0; k < numChannels ; k++)	//	iterate through channels
				{
					// change read order if necessary
					unsigned int sourceX = i;
					unsigned int sourceY = j;

					if ( !( header.imageDesc & TGA_DIRECTION_TOPTOBOTTOM ) )
						sourceY = ( ( unsigned int )header.height - sourceY ) - 1;
					if ( !( header.imageDesc & TGA_DIRECTION_RIGHTTOLEFT ) )
						sourceX = ( ( unsigned int )header.width - sourceX ) - 1;	// untested... don't have image written right to left.

					//	set copy source and dest locations
					unsigned int locSource =( ( sourceX + ( sourceY * (unsigned int)header.width ) ) * numChannels) + k;
					unsigned int locDest =( ( i + ( j * (unsigned int)header.width ) ) * numChannels) + k;

					//	copy the data to the tempData buffer.
					tempData[locDest] = imageData[locSource];
				}
			}
		}
			
		//put the loaded and converted image data into the return image.  Reminder tga is either BGRA or BGR
		unsigned char imageFormat;
		if ( numChannels == 3 )
		{
			imageFormat = IUI_FORMAT_BGR;
		}
		else // is 4 the only other option?
		{
			imageFormat = IUI_FORMAT_BGRA;
		}	
		returnImage.setData( imageFormat, header.width, header.height, numChannels, tempData);
		
			
		//do some cleanup
		free( tempData );	// allocated with malloc
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
	free( imageID );	// allocated with malloc
	imageID = nullptr;
	

	free( imageData );	// allocated with malloc
	imageData = nullptr;

	return returnImage;
}

IUImage<unsigned char> ImageUtilities::LoadPNG(const char* filename)
{
	IUImage<unsigned char> returnImage;

	std::ifstream inFile;	//input file stream for reading file.

	inFile.open (filename, std::ifstream::in | std::ifstream::binary);

	if (inFile.is_open())
	{
		// read the file header.  8 bytes
		unsigned char pngHeader[8];
		inFile.read((char*) &pngHeader, 8);

		// next start reading the chunks
		while( true ) // ... need stop condition
		{
			// each chunk starts with a 4 byte length
			unsigned int chunkLen;
			inFile.read( (char*) &chunkLen, sizeof( chunkLen ) );
			chunkLen = ReverseBytes( chunkLen, sizeof( chunkLen ) );  // stored big endian so reverse it

			// next get the chunk type

			unsigned char chunkType[4];
			inFile.read( (char*) &chunkType, sizeof( chunkType ) );

			// how we read the chunk will differ
			if( chunkType[0] == 'I' && chunkType[1] == 'H' && chunkType[2] == 'D' && chunkType[3] == 'R'  ) // IHDR
			{
				// IHDR. Image Header.
				PNGChunkIHDR chunkIHDR;

				// read the chunk.
				inFile.read( ( char* ) &chunkIHDR.width, sizeof( chunkIHDR.width ) );
				inFile.read( ( char* ) &chunkIHDR.height, sizeof( chunkIHDR.height ) );
				inFile.read( ( char* ) &chunkIHDR.bitDepth, sizeof( chunkIHDR.bitDepth ) );
				inFile.read( ( char* ) &chunkIHDR.colortype, sizeof( chunkIHDR.colortype ) );
				inFile.read( ( char* ) &chunkIHDR.compressionMethod, sizeof( chunkIHDR.compressionMethod ) );
				inFile.read( ( char* ) &chunkIHDR.filterMethod, sizeof( chunkIHDR.filterMethod ) );
				inFile.read( ( char* ) &chunkIHDR.interlaceMethod, sizeof( chunkIHDR.interlaceMethod ) );

				// reverse the bytes on the width/hight
				chunkIHDR.width = ReverseBytes( chunkIHDR.width, sizeof( chunkIHDR.width ) );
				chunkIHDR.height = ReverseBytes( chunkIHDR.height, sizeof( chunkIHDR.height ) );
			}
			else if( chunkType[0] == 'I' && chunkType[1] == 'D' && chunkType[2] == 'A' && chunkType[3] == 'T'  ) // IDAT
			{
				// IDAT. Image Data
				unsigned char* imageData = (unsigned char*)malloc( chunkLen );
				inFile.read( ( char* ) imageData, chunkLen );

				// get the compression method and info located in the first byte

				unsigned char cmf = imageData[0];
				unsigned char cm = cmf & 9;						// compression method: lower 4 bits... this should be 8
				unsigned char cinfo = cmf >> 4;					// compression info: upper 4 bits... bitshift to get these... this is the number bits used for the window size.  max is 7.
				unsigned int windowSize = pow( 2, cinfo + 8 );		// convert cinfo into proper window size.

				// get the flags located in the second byte

				unsigned char flg = imageData[1];
				unsigned char fCheck = flg & 17;				// check bits for cmf and flg (not sure what that means yet) bits 0-4
				unsigned char fDict = ( flg >> 5) & 1;			// has preset dictionary? ( for PNG this should always be 0 )  bit 5.
				unsigned char fLevel = ( flg >> 6 ) & 3;		// Compression Level... not needed if we are merely decompressing.

				// next comes the compressed (DEFLATE) data.

				// http://www.ietf.org/rfc/rfc1951.txt

				/*do
					read block header from input stream.
					if stored with no compression
						skip any remaining bits in current partially processed byte
						read LEN and NLEN (see next section)
						copy LEN bytes of data to output
					otherwise
						if compressed with dynamic Huffman codes
							read representation of code trees (see subsection below)
						loop (until end of block code recognized)
							decode literal/length value from input stream
							if value < 256
								copy value (literal byte) to output stream
							otherwise
								if value = end of block (256)
									break from loop
								otherwise (value = 257..285)
									decode distance from input stream

									move backwards distance bytes in the output
									stream, and copy length bytes from this
									position to the output stream.
						end loop
				while not last block*/

				// Create a BitReader Object to read the individual bits

				BitReader<unsigned char> imageBits( imageData, chunkLen );
				imageBits.Seek( 16 );  // set the read head at the beginning of the 3rd byte.
				
				// 
				unsigned int bFinal = 0;

				do
				{
					// read first byte
					//unsigned char firstByte = imageData[2];
					//unsigned char bFinal = firstByte & 1;
					// unsigned char bType = ( firstByte >> 1 ) & 3;

					// read header

					bFinal = imageBits.ReadBits( 1, false );	// first bit of header is set if its the final block.
					unsigned int bType = imageBits.ReadBits( 2, false );	// next to bits of header contain compression type.


					if ( bType == 0 )
					{
					}
					else
					{
						if( bType == 2)
						{
						}
						while ( true /* not end of block */ )
						{
							if ( bType == 1 )
							{
								// this is a hardcoded way of decoding fixed huffman tree based on value ranges.
								// should really but the info in a tree and traverse that.

								// grab first seven bits
								unsigned int code = imageBits.ReadBits( 7, true );
								
								int litVal;

								if ( code >= 0 && code <=23 )
								{
									litVal = code + 256;
								}
								else // not a seven bit code
								{
									code = ( code << 1 ) | imageBits.ReadBits( 1, false );

									if ( code >= 48 && code <= 199 )
									{
										if (code < 192)
										{
											litVal = code - 48;
										}
										else
										{
											litVal = code + ( 280 - 192 );
										}
									}
									else // not a 8 bit code.
									{
										code = ( code << 1 ) | imageBits.ReadBits( 1, false );

										if ( code >= 400 && code <= 511 )
										{
											litVal = code - ( 400 - 144 );
										}
										else
										{
											// something went wrong.
										}
									}

									
								}


								/*
								if ( litVal > 256 )
								{
									// we have a length... next five bits should be the dist
									unsigned int distCode = imageBits.ReadBits( 5, true );
									
									unsigned int distance;

									switch ( distCode )
									{
									case 0:
									case 1:
									case 2:
									case 3:
										distance = distCode + 1;
										break;
									case 4:
									case 5:
										// 1 extra bit
										unsigned int extraCode = imageBits.ReadBits( 1, true );
										distance = ( distCode + 1 ) + extraCode;
									case 6:
									case 7:
										// 2 extra bit
										unsigned int extraCode = imageBits.ReadBits( 1, true );
										distance = ( distCode + 1 ) + extraCode;

									}
								}*/

								litVal = litVal;
							}
						}
					}

				}
				while ( bFinal == 0 /* not last block */ );

				free (imageData);
			}
			else
			{
				// anything else just read past it.
				unsigned char* chunkData = (unsigned char*)malloc( chunkLen );
				inFile.read( (char*) chunkData, chunkLen );

				free( chunkData );
			}

			// read the CRC at the end of the chunk
			unsigned char chunkCRC[4];
			inFile.read( (char*) &chunkCRC, sizeof( chunkCRC ) );
			
		}
		



		//close the open file.
		inFile.close();
	}

	return returnImage;
}
