#ifndef BITREADER_H
#define BITREADER_H

/**
	BitReader
	Class to allow the reading of individual bits of a piece of data.  Can return up to 32 bits of data
	Author: Jeff Adams
*/

//#include <assert.h>

template <class T>
class BitReader
{
private:
	// Members

	unsigned int bitPosition;		// what bit are we pointing at. Counts from beginning of data
	const unsigned char* data;		// what we will be reading the bits from.
	unsigned int size;				// how big is the data... in bytes.
public:

	// Structors
	BitReader( const T* data, const unsigned int size );

	// Getters and Setters... do we need?

	// Functions

	/**
		ReadBits()
		Returns a specified number of bits starting from the current bitPosition and advances bitPosition.  Can return
		up to 32 bits at a time.
		@param numBits - Number of bits to return.  Max 32
		@return - the bits.

	*/
	unsigned int ReadBits( const char numBits, const bool reverse );

	/**
		ReadBitAt()
		Returns the value of a single bit at the specified location
		@param position - position of the bit to retrieve
		@return the value of the bit
	*/
	unsigned char ReadBitAt( const unsigned int position );

	/**
		Seek()
		Move bitPosition to specified location
	*/
	void Seek( const unsigned int position );

	/**
		Tell()
		Where is the read position
	*/
	unsigned int Tell() const;
};

// Structors

template <class T>
BitReader<T>::BitReader( const T* data, const unsigned int size )
{
	this->bitPosition = 0;
	this->data = (const unsigned char*)data;
	this->size = size;
}


// Functions

template <class T>
unsigned int BitReader<T>::ReadBits( const char numBits, const bool reverse )
{
	unsigned int bitBuffer = 0;		// temporary place to store the bits
	unsigned int returnBits = 0;

	// validate that there are enough bits left in data to read this number of bits
	if( bitPosition + numBits >= size * 8 || numBits >= 32 )
		throw "Out of Bounds";

	for( unsigned int i = 0; i < numBits; i++ )
	{
		// shift previous buffered bits over 1
		bitBuffer = bitBuffer << 1;

		// add next bit
		bitBuffer = bitBuffer | ReadBitAt( bitPosition );

		// advnace bitPosition
		bitPosition++;
	}

	// the bit buffer is backwards so reverse it... unless it is asked for in reversed order
	if( !reverse )
	{
		for( unsigned int i = 0; i < numBits; i++ )
		{
			returnBits = returnBits << 1;

			returnBits = returnBits | ( bitBuffer & 1);

			bitBuffer = bitBuffer >> 1;
		}
	}
	else
		returnBits = bitBuffer;

	return returnBits;
}

template <class T>
unsigned char BitReader<T>::ReadBitAt( const unsigned int position )
{
	unsigned char returnBit;

	// validate position
	if( position >= this->size * 8 )
		throw "Out of Bounds";
	
	// what byte the bit is in.
	unsigned int byteNumber = position / 8;
		
	// what is the position of the bit in the byte
	unsigned int bitNumber = position % 8;

	// get the byte
	unsigned char byteData = data[ byteNumber ];

	// shift the bits of the byte so that the bit we need is in the first position
	returnBit = ( byteData >> bitNumber ) & 1;
	
	return returnBit;
}

template <class T>
void BitReader<T>::Seek( const unsigned int position )
{
	// validate position
	if( position >= this->size * 8 )
		throw "Out of Bounds";

	this->bitPosition = position;
}

template <class T>
unsigned int BitReader<T>::Tell() const
{
	return bitPosition;
}

#endif /* BITREADER_H */