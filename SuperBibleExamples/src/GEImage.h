#ifndef GEIMAGE_H
#define GEIMAGE_H

/**
	GEImage
	Purpose: Definition of GEImage Class.  This class used to hold image data.
			
	@author Jeff Adams
*/

class GEImage
{
private:
	
	//private members
	unsigned int height;
	unsigned int width;
	unsigned char numChannels;	//3 or 4 -- really only need 2 bits to store this... but a char will work... its only 6 wasted bytes.
	unsigned int dataSize;		//total size in bytes for the image data.
	float* data;				//pointer to data. 
	 
public:
	
	//constructors
	GEImage();
	GEImage(const unsigned int height,const unsigned int width,const unsigned char bitDepth,const unsigned int dataSize,const float* data);
	GEImage(const GEImage& sourceImage);  // copy constructor

	//destructor
	~GEImage();

	//operators
	GEImage& operator=( GEImage sourceImage );	// assignment operator

	//setters
	void setHeight(const unsigned int height);
	void setWidth(const unsigned int width);
	void setNumChannels(const unsigned char numChannels);
	/**
		Purpose: Sets the data for the image and its size
		@param dataSize - Size of data in bytes of the data we are setting.  Calculated by Height * Width * numChannels * sizeof(float)
		@param data - pointer to the image data that will be copied to the private member data.
		@return void
	*/
	void setData(const unsigned int dataSize, const float* data);

	//getters
	unsigned int getHeight();
	unsigned int getWidth();
	unsigned char getNumChannels();
	/**
		Purpose: Returns the size of the data and fills the data in the parameter pointer.
		@param returnData - pointer to a copy of the data is filled in.
		@return size in bytes of image data
	*/
	unsigned int getData(float* returnData);
};

#endif /* GEIMAGE_H */