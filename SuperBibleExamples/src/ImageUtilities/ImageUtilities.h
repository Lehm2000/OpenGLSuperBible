#ifndef IMAGEUTILITIES_H
#define IMAGEUTILITIES_H

#include "IUImage.h"

//bitmap compression constants
#define BI_RGB 0
#define BI_RLE8 1
#define BI_RLE4 2
#define BI_BITFIELDS 3
#define BI_JPEG 4
#define BI_PNG 5
#define BI_ALPHABITFIELDS 6
#define BI_CMYK 11
#define BI_CMYKRLE8 12
#define BI_CMYKTLE4 13

//define bitmap header
struct BitmapHeader  //Header for all Bitmaps
{
	unsigned char fileType[2];		//identify what kind of bitmap
	unsigned int fileSize;			//size of file in bytes.
	unsigned char reserved1[2];		//not sure what goes in here.  TODO: find out what to do with this
	unsigned char reserved2[2];		//not sure what goes in here.  TODO: find out what to do with this
	int dataOffset;					//where does image info begin
};

struct BITMAPINFOHEADER //Bitmap info for Windows NT & 3.1x or later
{
	unsigned int headerSize;			// 	the size of this header (40 bytes)
	unsigned int width;					// 	the bitmap width in pixels (signed integer)
	unsigned int height;				// 	the bitmap height in pixels (signed integer)
	unsigned short colorPlanes;			//	the number of color planes must be 1
	unsigned short bitDepth;			// 	the number of bits per pixel, which is the color depth of the image. Typical values are 1, 4, 8, 16, 24 and 32.
	unsigned int compressionType;		// 	the compression method being used. See the next table for a list of possible values
	unsigned int imageSize;				// 	the image size. This is the size of the raw bitmap data; a dummy 0 can be given for BI_RGB bitmaps.
	unsigned int hRes;					// 	the horizontal resolution of the image. (pixel per meter, signed integer)
	unsigned int vRes;					// 	the vertical resolution of the image. (pixel per meter, signed integer)
	unsigned int numPaletteColors;		// 	the number of colors in the color palette, or 0 to default to 2n
	unsigned int numImportantColors;	// 	the number of important colors used, or 0 when every color is important; generally ignored
};

//	targa constants
#define TGA_NO_DATA					0
#define TGA_UNCOMPRESSED_PALETTE	1
#define TGA_UNCOMPRESSED_RGB		2
#define TGA_UNCOMPRESSED_BW			3
#define TGA_RLE_PALETTE				9
#define TGA_RLE_RGB					10
#define TGA_COMPRESSED_BW			11
#define TGA_COMPRESSED_PALETTE		32
#define TGA_COMPRESSED_PALETTE_QT	33

#define TGA_DIRECTION_RIGHTTOLEFT	8				// set bit 4
#define TGA_DIRECTION_TOPTOBOTTOM	16				// set bit 5

#define TGA_RLE_PACKET				128				// set bit 7



//targa header

struct TargaHeader
{
	unsigned char idLen;				//	Length of the image ID field
	unsigned char colorMapType;			//	Wheather a color map is included
	unsigned char imageType;			//	Compression and color types
	
	unsigned short colorMapFirstIndex;	//	Offset into the color map table
	unsigned short colorMapLength;		//	Number of entries
	unsigned char colorMapEntrySize;	//	Number of bits per pixel
	
	unsigned short xOrigin;				//	Absolute coordinate of lower-left corner for displayer where origin is at the lower left
	unsigned short yOrigin;				//	See above
	unsigned short width;				//	Image width;
	unsigned short height;				//	Image height;
	unsigned char bitDepth;				//	Pixel depth;
	unsigned char imageDesc;			//	bits 3-0 give the alpha channel depth, bits 5-4 give direction
};


//Image functions

class ImageUtilities
{
private:
public:
	IUImage<unsigned char> LoadBitmap(const char* filename);
	IUImage<unsigned char> LoadTarga(const char* filename);

};


#endif /*IMAGEUTILITIES_H */
