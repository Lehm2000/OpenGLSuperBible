/**
*/

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "IUImage.h"

//Constructors
IUImage::IUImage()
{
	width = 0;
	height = 0;
	numChannels = 0;
	data = nullptr;
}

IUImage::IUImage(unsigned int width, unsigned int height, unsigned int numChannels, unsigned char* data)
{
	this->width = width;
	this->height = height;
	this->numChannels = numChannels;
	this->data = data;
}

IUImage::IUImage(const IUImage& sourceImage)
{
	height = sourceImage.height;
	width = sourceImage.height;
	numChannels = sourceImage.numChannels;

	unsigned int dataSize = height * width * numChannels;

	//Allocated enough memory to hold the image and copy it in.
	data = (unsigned char*)malloc(dataSize);
	memcpy(data, sourceImage.data,dataSize);
}

//destructor
IUImage::~IUImage()
{
	if (data != nullptr)
	{
		delete[] data;
		data = nullptr;
	}
}

//operators
IUImage& IUImage::operator=( IUImage sourceImage )
{
	//we passed the sourceImage as a copy which called the copy constructor

	//now swap the copied members from the copied GEImage
	std::swap(height, sourceImage.height);
	std::swap(width, sourceImage.width);
	std::swap(numChannels, sourceImage.numChannels);
	std::swap(data, sourceImage.data);
	
	return *this;
}

//setters
void IUImage::setHeight(const unsigned int height)
{
	this->height = height;
}

void IUImage::setWidth(const unsigned int width)
{
	this->width = width;
}

void IUImage::setNumChannels(const unsigned char numChannels)
{
	this->numChannels = numChannels;
}

void IUImage::setData(const unsigned int dataSize, const unsigned char* data)
{
	//this->dataSize = dataSize;
	this->data = (unsigned char*)malloc(dataSize);
	memcpy(this->data, data, dataSize);
}

//getters
unsigned int IUImage::getHeight() const
{
	return this->height;
}

unsigned int IUImage::getWidth() const
{
	return this->width;
}

unsigned char IUImage::getNumChannels() const
{
	return this->numChannels;
}

void IUImage::getData(unsigned char* returnData) const
{
	//first delete any data that might be in there.  Otherwise it might be a leak.
	//if (returnData != nullptr)
		//delete[] returnData;

	//this next step could be dangerous if the size of the returnData is not the proper size. //TODO: find a way to make this safe. 
	memcpy( returnData, this->data, this->getDataSize() );
}

//functions
unsigned int IUImage::getDataSize() const
{
	return height * width * numChannels * sizeof(unsigned char);
}

