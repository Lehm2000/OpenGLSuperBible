#ifndef GECONTROLLERINPUTMOUSEPOSITIONX_H
#define GECONTROLLERINPUTMOUSEPOSITIONX_H

/**
	GEControllerInputMousePosition
	Purpose: GEController that monitors the X position of the mouse and applies the deltaVec as a multiple of the offset.
	Author: Jeff Adams
*/
#include <glm\glm.hpp>

#include "GEControllerConstant.h"

class GEControllerInputMousePositionX: public GEControllerConstant
{
private:
	
	float mousePositionX;			// where is the mouse
	float mousePositionXPrev;		// where was the mouse

public:
	GEControllerInputMousePositionX();
	GEControllerInputMousePositionX( const glm::vec3 deltaVec );
	GEControllerInputMousePositionX( const GEControllerInputMousePositionX& source );
	virtual ~GEControllerInputMousePositionX();

	// Setters
	virtual void setGameEntities( const std::map< std::string, GEObject* >* gameEntities );  // override from GEController

	// Functions

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEControllerInputMousePositionX* clone() const;

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


#endif /* GECONTROLLERINPUTMOUSEPOSITIONX_H */