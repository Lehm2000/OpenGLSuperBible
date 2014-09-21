#ifndef GECONTROLLERINPUTKEY_H
#define GECONTROLLERINPUTKEY_H

/**
	GEControllerInputKey
	Purpose: GEController that monitors the status of a specified key and applies the specifed deltaVec to it.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"

class GEControllerInputKey: public GEControllerConstant
{
private:
	unsigned short key;		// key to listen for.
	bool pressed;			// is the key pressed
	bool pressedPrev;		// was the key pressed the last time around.

public:
	GEControllerInputKey();
	GEControllerInputKey( const glm::vec3 deltaVec, unsigned short key );
	GEControllerInputKey( const GEControllerInputKey& source );
	virtual ~GEControllerInputKey();

	// Setters
	void setKey( unsigned short key );

	// Getters
	unsigned int getKey() const;

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerInputKey* clone() const;

	/**
		Update()
		Takes objectVector and applies the deltaVec to it if this objects key is pressed.
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


#endif /* GECONTROLLERINPUTKEY_H */