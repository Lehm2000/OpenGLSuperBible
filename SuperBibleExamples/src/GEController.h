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

#include <map>
#include <glm\glm.hpp>

#include "GEObject.h"

class GEObject;

class GEController
{
protected:
	// Members
	
	const GEObject* parent;  // in case the controller needs to access the properties of the parent.  Like the lookat controller needs to know the parents position for example.
	const std::map< std::string, GEObject* >* gameEntities;  // pointer to the master gameEntity list.  In case the controller needs to know the properties of some other object in the world.  This once scares me a bit... I know its const... but is there a better way to get this info?

	glm::vec3 transformedVector;	// the transformed data.

public:
	// Structors
	
	GEController();
	GEController( const GEObject* parent, const std::map< std::string, GEObject* >* gameEntities );
	GEController( const GEController& source);
	virtual ~GEController();

	// Setters

	virtual void setParent( const GEObject* parent );
	virtual void setGameEntities( const std::map< std::string, GEObject* >* gameEntities );

	// Getters

	virtual const GEObject* getParent() const;
	virtual const std::map< std::string, GEObject* >* getGameEntities() const;

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
	virtual void Control( glm::vec3 objectVector, double gameTime, double deltaTime );

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual glm::vec3 CalcTransform( glm::vec3 sourceVector ); 
};


#endif /* GECONTROLLER_H */