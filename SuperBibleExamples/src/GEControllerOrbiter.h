#ifndef GECONTROLLERORBITER_H
#define GECONTROLLERORBITER_H

/**
	GEControllerOrbit
	Controller designed to revolve an object around another.  Therefore best used in the position property.

	Author: Jeff Adams
*/

#include <string>
#include <glm\gtx\rotate_vector.hpp>

#include "GEControllerLookAt.h"
#include "TypeDefinitions.h"
#include "GEProperty.h"

class GEControllerOrbiter: public GEControllerLookAt
{
private:
	// Members

	GEPropertyv3 orbitAngle;			// orbit angle
	GEPropertyf1 orbitDistance;			// how far are we from the target.

public:
	GEControllerOrbiter();
	GEControllerOrbiter( const std::string targetName, GEvec3 initialOrbitAngle, float orbitDistance );
	GEControllerOrbiter( const GEControllerOrbiter& source );
	virtual ~GEControllerOrbiter();

	// Setters
	virtual void setGameEntities( const GEObjectContainer* gameEntities );  // this must be overridden if we have new GEProperties.

	// Getters

	GEPropertyv3* getOrbitAngleProp();
	const GEPropertyv3* getOrbitAngleProp() const;
	GEPropertyf1* getOrbitDistanceProp();
	const GEPropertyf1* getOrbitDistanceProp() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerOrbiter* clone() const;

	/**
		Update()
		Takes objectVector and applies the deltaVec to it.
		@param startVector - starting point of the object.  Could be position, rotation or scale
		@param gameTime - time since the game started
		@param deltaTime - time since the last frame
		@return
	*/
	virtual GEvec3 Control( const GEvec3 prevValue, const double gameTime, const double deltaTime, GEvec3 max, bool useMax, GEvec3 min, bool useMin );

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual GEvec3 CalcTransform( const GEvec3 sourceVector );

};

GEControllerOrbiter::GEControllerOrbiter()
	:GEControllerLookAtv3()
{
	this->orbitAngle.setValue( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->orbitDistance.setValue( 0.0f );
}

GEControllerOrbiter::GEControllerOrbiter(  const std::string targetName, GEvec3 initialOrbitAngle, float orbitDistance  )
	:GEControllerLookAtv3( targetName )
{
	this->orbitAngle.setValue( initialOrbitAngle );
	this->orbitDistance.setValue( orbitDistance );
}

GEControllerOrbiter::GEControllerOrbiter( const GEControllerOrbiter& source )
	:GEControllerLookAtv3( source.targetName )
{
	this->orbitAngle = source.orbitAngle;
	this->orbitDistance = source.orbitDistance;
}

GEControllerOrbiter::~GEControllerOrbiter()
{
}

void GEControllerOrbiter::setGameEntities( const GEObjectContainer* gameEntities )
{
	// set this objects gameEntities pointer
	this->gameEntities = gameEntities;

	// then go through the other properties too.
	orbitAngle.setControllerGameEntitiesPointer( gameEntities );
	orbitDistance.setControllerGameEntitiesPointer( gameEntities );
}

// Getters

GEPropertyv3* GEControllerOrbiter::getOrbitAngleProp()
{
	return &this->orbitAngle;
}

const GEPropertyv3* GEControllerOrbiter::getOrbitAngleProp() const
{
	return &this->orbitAngle;
}

GEPropertyf1* GEControllerOrbiter::getOrbitDistanceProp()
{
	return &this->orbitDistance;
}

const GEPropertyf1* GEControllerOrbiter::getOrbitDistanceProp() const
{
	return &this->orbitDistance;
}



// Functions


GEControllerOrbiter* GEControllerOrbiter::clone() const
{
	return new GEControllerOrbiter( *this );
}



GEvec3 GEControllerOrbiter::Control( const GEvec3 prevValue, const double gameTime, const double deltaTime, GEvec3 max, bool useMax, GEvec3 min, bool useMin )
{
	// We need to calculate the new position based on where the target is, how far from the target it should be, and the current rotation angle.

	// update the properties first.
	orbitAngle.Update( gameTime, deltaTime);
	orbitDistance.Update( gameTime, deltaTime);
		
	// get a reference to the target.
	const GEObject* targetObject = gameEntities->GetObject( this->targetName );
	
	if ( targetObject != nullptr )
	{
		const GEvec3 targetPos = targetObject->getPosition()->getFinalValue();
		
		// Create a new starting vector based on the distance.
		glm::vec4 distVector( 0.0f, 0.0f, orbitDistance.getFinalValue(), 1.0f );

		// create a rotation matrix and rotate the distVector by it.
		glm::vec4 newPos = glm::rotate( glm::mat4(), orbitAngle.getFinalValue().z, glm::vec3( 0.0f, 0.0f, 1.0f ) ) * glm::rotate( glm::mat4(), orbitAngle.getFinalValue().y, glm::vec3( 0.0f, 1.0f, 0.0f ) ) * glm::rotate( glm::mat4(), orbitAngle.getFinalValue().x, glm::vec3( 1.0f, 0.0f, 0.0f ) ) * distVector;
		
		// next transform the position to target.
		transformedValue = targetPos + glm::vec3( newPos.x, newPos.y, newPos.z );

		// assign the new calculated rotation back... this ignores what we were given.
		//transformedValue = newPos;
	}
	else
	{
		// just return what we were given
		transformedValue = prevValue;
	}

	// this control function ignores all incoming values... so pass a new vec3 for the prevValue
	transformedValue = ValidateRange( transformedValue, GEvec3( 0.0f, 0.0f, 0.0f ), max, useMax, min, useMin );

	return transformedValue;
}



GEvec3 GEControllerOrbiter::CalcTransform( const GEvec3 sourceVector )
{
	return transformedValue;  // ignore the sourceVector
}

typedef GEControllerOrbiter GEControllerOrbiterv3;  // only version is a vec3

#endif /* GECONTROLLERROTATOR_H */