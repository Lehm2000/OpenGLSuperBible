#ifndef GECONTROLLEROSCILLATOR_H
#define GECONTROLLEROSCILLATOR_H

/**
	GEControllerConstant
	Purpose: GEController object that applies a constant change to the object
	Author: Jeff Adams
*/

#include <glm\glm.hpp>

#include "GEController.h"

class GEControllerOscillator: public GEController
{
private:
	// Members

	glm::vec3 amplitude;  // Position, Rotation (Radians) or Scale change per second 
	float frequency;	// how often (in seconds)

public:
	//Structors

	GEControllerOscillator();
	GEControllerOscillator( glm::vec3 amplitude, float frequency );
	GEControllerOscillator( const GEControllerOscillator& source );
	virtual ~GEControllerOscillator();

	// Setters
	void setAmplitude( const glm::vec3 amplitude );
	void setFrequency( const float frequency );

	// Getters
	glm::vec3 getAmplitude() const;
	float getFrequency() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerOscillator* clone() const;

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

#endif /* GECONTROLLEROSCILLATOR_H */