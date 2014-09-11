

#include "GEControllerLookAt.h"
#include "GEConstants.h"

//Structors

GEControllerLookAt::GEControllerLookAt()
{
}

GEControllerLookAt::GEControllerLookAt( const std::string targetName )
	:GEController()
{
	this->targetName = targetName;
}

GEControllerLookAt::GEControllerLookAt( const GEControllerLookAt& source )
	:GEController( source.parent, source.gameEntities )
{
	this->targetName = source.targetName;
}

GEControllerLookAt::~GEControllerLookAt()
{

}





// Functions

GEControllerLookAt* GEControllerLookAt::clone() const
{
	return new GEControllerLookAt( *this );
}

void GEControllerLookAt::Control( glm::vec3 objectVector, double gameTime, double deltaTime)
{
	//transformedVector = sin( (float)gameTime / frequency ) * amplitude;

	// We need to calculate the x and y rotations.  The z rotation is left alone to allow tilt/roll.

	// Grab the existing rotation
	glm::vec3 newRot( 0.0f, 0.0f, 0.0f );
		
	// Get the delta of the camera and target positions
	//glm::vec3 deltaVec = getTargetPosition() - getPosition();
	std::map< std::string, GEObject* >::const_iterator targetIt = gameEntities->find( this->targetName );
	
	if ( targetIt != gameEntities->end() && parent != nullptr )
	{
	
		glm::vec3 targetPos = targetIt->second->getTransformedPosition();
		glm::vec3 parentPos = parent->getTransformedPosition();
		glm::vec3 deltaVec = targetPos - parentPos;

		// get the distance in the xz plane
		float dist = glm::distance( glm::vec3(targetPos.x, 0.0, targetPos.z), glm::vec3(parentPos.x, 0.0, parentPos.z) );

		//newRot = glm::atan2(deltaVec);
		
		// first x (pitch) rotation
		if ( deltaVec.y == 0.0f && dist == 0.0f )  // If points are the same.
		{
			newRot.x = 0.0f;
		}
		else
		{
			newRot.x = atan2( deltaVec.y, dist );
		}

		// now y (yaw) rotation
		if ( deltaVec.x == 0.0f && deltaVec.z == 0.0f )  // If points are the same.
		{
			newRot.y = 0.0f;
		}
		else
		{
			newRot.y = ( atan2( deltaVec.z * -1.0,  deltaVec.x  ) ) - ( GE_PI / 2.0 );  // this works... but the math looks ugly... replace atan2?
		
		}

		// We don't touch the z...for now.
		
		// assign the new calculated rotation back... this ignores what we were given.
		transformedVector = newRot;
	}
	else
	{
		// just return what we were given
		transformedVector = objectVector;
	}
}


glm::vec3 GEControllerLookAt::CalcTransform( glm::vec3 sourceVector )
{
	return transformedVector;  // ignore the sourceVector
}
