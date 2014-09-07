#ifndef GECONTROLLERCONSTANT_H
#define GECONTROLLERCONSTANT_H

/**
	GEControllerConstant
	Purpose: GEController object that applies a constant change to the object
	Author: Jeff Adams
*/

#include <glm\glm.hpp>

#include "GEController.h"

class GEControllerConstant: public GEController
{
private:
	// Members

	glm::vec3 deltaVec;  // Position, Rotation (Radians) or Scale change per second 

public:
	//Structors

	GEControllerConstant();
	GEControllerConstant(glm::vec3 deltaVec);
	virtual ~GEControllerConstant();

	// Setters
	void setDeltaVec( const glm::vec3 deltaVec );

	// Getters
	glm::vec3 getDeltaVec() const;

	// Functions

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

#endif /* GECONTROLLERCONSTANT_H */