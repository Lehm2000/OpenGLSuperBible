#ifndef GECONTROLLER_H
#define GECONTROLLER_H

/**
	GEController Class
	
	Purpose: 
	This is the base controller class for objects in the game.
	Controllers are the means by which objects are moved, rotated and scaled.
	The base class merely returns what was passed to it.  The object it is
	attached to basically is static then.


	Author: Jeff Adams
*/

#include <glm\glm.hpp>


class GEController
{
protected:
	// Members
	
	glm::vec3 transformedVector;	// the transformed data.

public:
	// Structors
	
	GEController();
	GEController( const GEController& source);
	virtual ~GEController();

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEController* clone() const;

	/**
		Control()
		Takes objectVector and passed it on without modification.
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


#endif /* GECONTROLLER_H */