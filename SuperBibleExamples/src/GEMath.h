#ifndef GEMATH_H
#define GEMATH_H

/**
	GEMath (Game Engine Math)
	Purpose:  To exercise writing my own math class.  Provides vector and matrix types and misc 3d functions
	Author: Jeff Adams

	History
	======================================
	2014-08-25
	Initial Version
*/

namespace gemath
{
	// going to hardcode it has float.  TODO convert to template
	class vecX
	{
	protected:
		char size;		// supports sizes up to 255
		float* data;
	public:
		//Structors
		vecX() 
		{
			size = 0;
			data = nullptr;
		}
		vecX( float v1 )
		{
			size = 1;
			data = new float[1];
			data[0] = v1;
		}
		vecX( float v1, float v2 )
		{
			size = 2;
			data = new float[2];
			data[0] = v1;
			data[1] = v2;
		}
		vecX( float v1, float v2, float v3 )
		{
			size = 3;
			data = new float[3];
			data[0] = v1;
			data[1] = v2;
			data[2] = v3;
		}
		vecX( float v1, float v2, float v3, float v4 )
		{
			size = 4;
			data = new float[4];
			data[0] = v1;
			data[1] = v2;
			data[2] = v3;
			data[3] = v4;
		}
		vecX( char size, float* data)
		{
			this->size = size;
			this->data = new float[size];
			for (char i = 0; i < size; i++)
			{
				this->data[i] = data[i];
			}
		}
		~vecX()
		{
			if ( data != nullptr )
			{
				delete[] data;
				data = nullptr;
			}
		}

		// Math Functions
		float operator[] ( char index )
		{
			return this->data[ index ];  // What hapens if index is greater than size?
		}
	}
};

#endif /* GEMATH_H */