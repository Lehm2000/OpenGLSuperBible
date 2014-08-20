
/**
	GEImage.cpp
	Purpose: Implementation of GEImage Class.  This class used to hold image data.
			
	@author Jeff Adams
*/

#include <cstdlib>
#include <cstring>
#include <algorithm>

#include "GEImage.h"


GEImage::GEImage()
{
	height = 0;
	width = 0;
	numChannels = 0;
	dataSize = 0;
	data = nullptr;	
}

GEImage::GEImage(const unsigned int height,const unsigned int width,const unsigned char numChannels,const unsigned int dataSize,const float* data)
{
	//TODO: validate
	this->height = height;
	this->width = width;
	this->numChannels = numChannels;
	this->dataSize = dataSize;

	this->data = (float*)malloc(dataSize);
	memcpy(this->data, data, dataSize);
}

GEImage::GEImage(const GEImage& sourceImage)
{
	height = sourceImage.height;
	width = sourceImage.height;
	numChannels = sourceImage.numChannels;
	dataSize = sourceImage.dataSize;

	//Allocated enough memory to hold the image and copy it in.
	data = (float*)malloc(dataSize);
	memcpy(data, sourceImage.data,dataSize);
}


GEImage::~GEImage()
{
	//cleanup image data
	delete[] data;
	data = nullptr;
}


GEImage& GEImage::operator=( GEImage sourceImage )
{
	//we passed the sourceImage as a copy which called the copy constructor

	//now swap the copied members from the copied GEImage
	std::swap(height, sourceImage.height);
	std::swap(width, sourceImage.width);
	std::swap(numChannels, sourceImage.numChannels);
	std::swap(dataSize, sourceImage.dataSize);
	std::swap(data, sourceImage.data);
	
	return *this;
}

//setters
void GEImage::setHeight(const unsigned int height)
{
	this->height = height;
}

void GEImage::setWidth(const unsigned int width)
{
	this->width = width;
}

void GEImage::setNumChannels(const unsigned char numChannels)
{
	this->numChannels = numChannels;
}

void GEImage::setData(const unsigned int dataSize, const float* data)
{
	this->dataSize = dataSize;
	this->data = (float*)malloc(dataSize);
	memcpy(this->data, data, dataSize);
}

//getters
unsigned int GEImage::getHeight() const
{
	return this->height;
}

unsigned int GEImage::getWidth() const
{
	return this->width;
}

unsigned char GEImage::getNumChannels() const
{
	return this->numChannels;
}

unsigned int GEImage::getData(float* returnData) const
{
	//first delete any data that might be in there.  Otherwise it might be a leak.
	if (returnData != nullptr)
		delete[] returnData;

	returnData = (float*)malloc(this->dataSize);
	memcpy(returnData,this->data,this->dataSize);

	return this->dataSize;
}