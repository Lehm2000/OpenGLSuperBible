#ifndef _IUIMAGE_H
#define _IUIMAGE_H

/**
*/

class IUImage
{
private:
	unsigned int width;			//image width
	unsigned int height;		//image height
	unsigned int numChannels;		//number of channels
	//unsigned int bufferSize;	//size in bytes of the data buffer.  Left out for now as it should be product of width height and channels.
	unsigned char* data;		//pointer to the data

public:

	//Constructors
	IUImage();
	IUImage(unsigned int width, unsigned int height, unsigned int channels, unsigned char* data);
	IUImage(const IUImage& sourceImage);  // copy constructor

	//destructor
	~IUImage();

	//operators
	IUImage& operator=( IUImage sourceImage );	// assignment operator

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
	void setData(const unsigned int dataSize, const unsigned char* data);

	//getters
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	unsigned char getNumChannels() const;
	/**
		Purpose: Returns the size of the data and fills the data in the parameter pointer.
		@param returnData - pointer to a copy of the data is filled in.
		@return size in bytes of image data
	*/
	void getData(unsigned char* returnData) const;

	//functions
	unsigned int getDataSize() const;
};


#endif /* IUIMAGE_H */