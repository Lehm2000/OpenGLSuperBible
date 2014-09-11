#ifndef GECONTROLLERLOOKAT_H
#define GECONTROLLERLOOKAT_H

/**
	GEControllerLookAt
	Purpose: GEController object that compares a target position and compares 
	it to the parent position and returns angles necessary to point at the 
	target.  Best used as a rotation controller.  Odd things would occur if
	used as another type of controller.
	Author: Jeff Adams
*/

#include <string>
#include <glm\glm.hpp>

#include "GEController.h"
#include "GEObject.h"

class GEControllerLookAt: public GEController
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

#endif /* GECONTROLLERLOOKAT_H */