
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
	

	// We must go through each of the 6 sides of the boundingbox and see if the ray hits any of them.
	// This probably isn't the fastest way... but it was fairly straight forward to implement.
	// TODO investigate faster ways.

	GEPlane iPlane;
	GEvec3* tempIPos = nullptr; 
	GEvec3* intersectPos = nullptr;

	float hitDist = 0.0f;  // temporary place to store how far away from the ray origin the hit happened.

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
			
			float newHitDist = glm::distance( ray.getOrigin(), *tempIPos ) ;

			// it hit the plane... check if it hit plane within bounds of box.  Don't check the bounds of the axis of the
			// current plane.  It can result in false negatives for thin objects.  Besides if it hit this plane we already
			// know its within the bounds of this axis anyway.  

			bool inBounds;

			if( i == 0 || i == 1 )
			{
				inBounds = (*tempIPos).y >= min.y && (*tempIPos).y <= max.y &&
					(*tempIPos).z >= min.z && (*tempIPos).z <= max.z ;
			}
			else if( i == 2 || i == 3 )
			{
				inBounds = (*tempIPos).x >= min.x && (*tempIPos).x <= max.x &&
					(*tempIPos).z >= min.z && (*tempIPos).z <= max.z ;
			}
			else
			{
				inBounds = (*tempIPos).x >= min.x && (*tempIPos).x <= max.x &&
					(*tempIPos).y >= min.y && (*tempIPos).y <= max.y ;
			}
			
			
			if( inBounds )
			{
				// did we already find an intsection?
				if( intersectPos != nullptr)
				{
					// compare new distance to old and see which is closer

					if( newHitDist < hitDist )
					{
						delete intersectPos;  // prevent memory leaking
						intersectPos = tempIPos;
						// TODO if this is our second hit should we break out... in theory it can only hit a cube twice.
					}
					// otherwise keep the existing pointer.
					
				}
				else
				{
					hitDist = newHitDist;
					intersectPos = tempIPos;
				}
			}
			
		}
	}

	return intersectPos;
}
