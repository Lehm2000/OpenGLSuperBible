#ifndef _IUIMAGE_H
#define _IUIMAGE_H

/**
	IUImage (Image Utilities Image) class.
	Purpose: Templated class for returning data from loaded images.  Uncompresses images when loaded.
	Author: Jeff Adams
*/

#define IUI_FORMAT_NONE		0
#define IUI_FORMAT_RGB		1
#define IUI_FORMAT_RGBA		2
#define IUI_FORMAT_BGR		3
#define IUI_FORMAT_BGRA		4


template <class T>
class IUImage
{
private:
	unsigned char format;		// Format of the image Member of IUI_FORMAT_*
	unsigned int width;			//image width
	unsigned int height;		//image height
	unsigned char numChannels;		//number of channels
	//unsigned int bufferSize;	//size in bytes of the data buffer.  Left out for now as it should be product of width height and channels.
	T* data;		//pointer to the data

public:

	//Constructors
	IUImage();
	IUImage(unsigned char format, unsigned int width, unsigned int height, unsigned char channels, T* data);
	IUImage(const IUImage<T>& sourceImage);  // copy constructor

	//destructor
	~IUImage();

	//operators
	IUImage& operator=( IUImage sourceImage );	// assignment operator

	//setters

	void setFormat( const unsigned char format );

	// void setHeight(const unsigned int height);
	// void setWidth(const unsigned int width);
	// void setNumChannels(const unsigned char numChannels);
	/**
		Purpose: Sets the data for the image and its size
		@param dataSize - Size of data in bytes of the data we are setting.  Calculated by Height * Width * numChannels * sizeof(float)
		@param data - pointer to the image data that will be copied to the private member data.
		@return void
	*/
	void setData( const unsigned char format, const unsigned int width, const unsigned int height, const unsigned char channels, const T* data);

	//getters
	unsigned char getFormat( ) const;
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	unsigned char getNumChannels() const;
	/**
		Purpose: Returns the size of the data and fills the data in the parameter pointer.
		@param returnData - pointer to a copy of the data is filled in.
		@return size in bytes of image data
	*/
	void getData(T* returnData) const;

	//functions
	unsigned int getDataSize() const;
};

//Constructors

//Constructors

template <class T>
IUImage<T>::IUImage()
{
	format = IUI_FORMAT_NONE;
	width = 0;
	height = 0;
	numChannels = 0;
	data = nullptr;
}

template <class T>
IUImage<T>::IUImage( unsigned char format, unsigned int width, unsigned int height, unsigned char numChannels, T* data)
{
	this->format = format;
	this->width = width;
	this->height = height;
	this->numChannels = numChannels;
	this->data = data;
}

template <class T>
IUImage<T>::IUImage(const IUImage& sourceImage)
{
	this->format = sourceImage.format;
	height = sourceImage.height;
	width = sourceImage.height;
	numChannels = sourceImage.numChannels;

	unsigned int dataSize = this->getDataSize();

	//Allocated enough memory to hold the image and copy it in.
	data = (T*)malloc( dataSize );
	memcpy( data, sourceImage.data, dataSize );
}

//destructor
template <class T>
IUImage<T>::~IUImage()
{
	free (data);
	data = nullptr;
}

//operators
template <class T>
IUImage<T>& IUImage<T>::operator=( IUImage<T> sourceImage )
{
	//we passed the sourceImage as a copy which called the copy constructor

	//now swap the copied members from the copied GEImage
	std::swap( format, sourceImage.format );
	std::swap(height, sourceImage.height);
	std::swap(width, sourceImage.width);
	std::swap(numChannels, sourceImage.numChannels);
	std::swap(data, sourceImage.data);
	
	return *this;
}

//setters

template <class T>
void IUImage<T>::setFormat( const unsigned char format )
{
	this->format = format;
}

/*
template <class T>
void IUImage<T>::setHeight(const unsigned int height)
{
	this->height = height;
}

template <class T>
void IUImage<T>::setWidth(const unsigned int width)
{
	this->width = width;
}

template <class T>
void IUImage<T>::setNumChannels(const unsigned char numChannels)
{
	this->numChannels = numChannels;
}
*/

template <class T>
void IUImage<T>::setData(const unsigned char format, const unsigned int width, const unsigned int height, const unsigned char channels, const T* data)
{
	this->format = format;
	this->width = width;
	this->height = height;
	this->numChannels = channels;

	// free old data if it has been allocated.
	free( this->data );
	this->data = nullptr;
	
	unsigned int dataSize = this->getDataSize();
	
	this->data = (T*)malloc( dataSize );
	memcpy(this->data, data, dataSize );
}

// getters

template <class T>
unsigned char IUImage<T>::getFormat( ) const
{
	return this->format;
}

template <class T>
unsigned int IUImage<T>::getHeight() const
{
	return this->height;
}

template <class T>
unsigned int IUImage<T>::getWidth() const
{
	return this->width;
}

template <class T>
unsigned char IUImage<T>::getNumChannels() const
{
	return this->numChannels;
}

template <class T>
void IUImage<T>::getData(T* returnData) const
{
	//this next step could be dangerous if the size of the returnData is not the proper size. //TODO: find a way to make this safe. 
	memcpy( returnData, this->data, this->getDataSize() );
}

//functions
template <class T>
unsigned int IUImage<T>::getDataSize() const
{
	return height * width * numChannels * sizeof(T);
}


#endif /* _IUIMAGE_H */