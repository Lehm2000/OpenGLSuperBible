
#include "GEBoundingBox.h"
#include "GERay.h"
#include "GEPlane.h"

GEBoundingBox::GEBoundingBox()
{
	this->setMin( GEvec3( -1.0f, -1.0f, -1.0f ));
	this->setMin( GEvec3( 1.0f, 1.0f, 1.0f ));
}

GEBoundingBox::GEBoundingBox( GEvec3 min, GEvec3 max )
{
	this->setMin( min );
	this->setMax( max );
}

// copy constructor if needed
// destructor if needed

// setters and getters
void GEBoundingBox::setMin( const GEvec3 min )
{
	this->min = min;
}

GEvec3 GEBoundingBox::getMin() const
{
	return this->min;
}


void GEBoundingBox::setMax( const GEvec3 max )
{
	this->max = max;
}

GEvec3 GEBoundingBox::getMax() const
{
	return this->max;
}

// Functions

GEvec3* GEBoundingBox::intersectRay( const GERay ray )
{
	GEvec3* intersectPos = nullptr;

	// We must go through each of the 6 sides of the boundingbox and see if the ray hits any of them.

	GEPlane iPlane;
	GEvec3* tempIPos = nullptr; 
	
	// x-min plane

	for( char i = 0; i < 6; i++ )
	{

		switch( i )
		{
		case 0:
			// x-min
			iPlane.setPlane( GEvec3( this->min.x, this->min.y, this->min.z), 
				GEvec3( this->min.x, this->max.y, this->min.z), 
				GEvec3( this->min.x, this->min.y, this->max.z) );
			break;
		case 1:
			// x-max
			iPlane.setPlane( GEvec3( this->max.x, this->min.y, this->min.z),
				GEvec3( this->max.x, this->max.y, this->min.z), 
				GEvec3( this->max.x, this->min.y, this->max.z) );
			break;
		case 2:
			// y-min
			iPlane.setPlane( GEvec3( this->min.x, this->min.y, this->min.z), 
				GEvec3( this->max.x, this->min.y, this->min.z), 
				GEvec3( this->min.x, this->min.y, this->max.z) );
			break;
		case 3:
			// y-max
			iPlane.setPlane( GEvec3( this->min.x, this->max.y, this->min.z), 
				GEvec3( this->max.x, this->max.y, this->min.z), 
				GEvec3( this->min.x, this->max.y, this->max.z) );
			break;
		case 4:
			// z-min
			iPlane.setPlane( GEvec3( this->min.x, this->min.y, this->min.z), 
				GEvec3( this->min.x, this->max.y, this->min.z), 
				GEvec3( this->max.x, this->min.y, this->min.z) );
			break;
		case 5:
			// z-max
			iPlane.setPlane( GEvec3( this->min.x, this->min.y, this->max.z), 
				GEvec3( this->min.x, this->max.y, this->max.z), 
				GEvec3( this->max.x, this->min.y, this->max.z) );
		}

		tempIPos = iPlane.intersectRay( ray );

		// did it hit plane?
		if( tempIPos != nullptr )
		{
			// check if it hit plane within bounds of box
			if( (*tempIPos).x >= min.x && (*tempIPos).x <= max.x &&
				(*tempIPos).y >= min.y && (*tempIPos).y <= max.y &&
				(*tempIPos).z >= min.z && (*tempIPos).z <= max.z )
			{

				if( intersectPos != nullptr)
				{
					// compare distance and see which is closer
					// TODO... could be memory leak here if we don't delete the pointer
					intersectPos = glm::distance( ray.getOrigin(), *tempIPos ) < glm::distance( ray.getOrigin(), *intersectPos ) ? tempIPos : intersectPos;
				}
				else
				{
					intersectPos = tempIPos;
				}
			}
		}
	}

	return intersectPos;
}
