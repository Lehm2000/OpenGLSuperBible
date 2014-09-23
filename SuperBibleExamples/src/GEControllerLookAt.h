#ifndef GECONTROLLERLOOKAT_H
#define GECONTROLLERLOOKAT_H

/**
	GEControllerLookAt
	Purpose: GEController object that compares a target position and compares 
	it to the parent position and returns angles necessary to point at the 
	target.  Best used as a rotation controller.  Odd things would occur if
	used as another type of controller.  Because this needs a vec3 this is
	not a template class.  It inherits GEController as a vec3
	Author: Jeff Adams
*/

#include <string>
#include <glm\glm.hpp>

#include "GEController.h"
#include "GEObject.h"

class GEControllerLookAt: public GEController<glm::vec3>
{
private:
	// Members

	std::string targetName;	// What are we looking at.

public:
	//Structors

	GEControllerLookAt();
	GEControllerLookAt( const std::string targetName );
	GEControllerLookAt( const GEControllerLookAt& source );
	virtual ~GEControllerLookAt();

	// Setters
	void setTargetName( const std::string targetName );

	// Getters
	std::string getTargetName() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerLookAt* clone() const;

	/**
		Update()
		Takes objectVector and applies the deltaVec to it.
		@param startVector - starting point of the object.  Could be position, rotation or scale
		@param gameTime - time since the game started
		@param deltaTime - time since the last frame
		@return
	*/
	virtual void Control( glm::vec3 objectVector, double gameTime, double deltaTime);

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual glm::vec3 CalcTransform( glm::vec3 sourceVector );

};



#include "GEControllerLookAt.h"
#include "GEConstants.h"

//Structors

GEControllerLookAt::GEControllerLookAt()
{
}

GEControllerLookAt::GEControllerLookAt( const std::string targetName )
	:GEController<glm::vec3>()
{
	this->targetName = targetName;
}

GEControllerLookAt::GEControllerLookAt( const GEControllerLookAt& source )
	:GEController<glm::vec3>( source.parent, source.gameEntities )
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
		transformedValue = newRot;
	}
	else
	{
		// just return what we were given
		transformedValue = objectVector;
	}
}


glm::vec3 GEControllerLookAt::CalcTransform( glm::vec3 sourceVector )
{
	return transformedValue;  // ignore the sourceVector
}

typedef GEControllerLookAt GEControllerLookAtv3;

#endif /* GECONTROLLERLOOKAT_H */