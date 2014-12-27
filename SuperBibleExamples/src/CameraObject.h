#ifndef CAMERAOBJECT_H
#define CAMERAOBJECT_H

/**
	CameraObject Class
	Purpose: Base class for camera objects in the world.  Used for setting the views in the game.  Extends GEObject

	Author: Jeff Adams

	Changes:
	
	2014-08-23
	Initial Version

*/

#include <glm\glm.hpp>


#include "GEObject.h"
#include "TypeDefinitions.h"

#define CAMTYPE_PERSPECTIVE				1
#define CAMTYPE_ORTHO					2
#define CAMTYPE_2D						3	// Is this one needed?  This one likely can be created with controllers.

class CameraObject: public GEObject
{
private:
	// Members
	
	// Target info
	
public:
	// Structors
	CameraObject();
	CameraObject( const CameraObject& source);
	CameraObject( GEvec3 position, GEvec3 rotation );
	~CameraObject();


	// Functions
	virtual std::string getClassName() const;

	/**
	 * Calculates the view matrix for the mesh
	 * 
	 * @param gameEntities - A reference to the gameEntities container.  Needed for some object types.
	 */
	virtual GEmat4 GetViewMatrix( const GEObjectContainer* gameEntities ) const;

	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual CameraObject* clone() const;

};


#endif /* CAMERAOBJECT_H */