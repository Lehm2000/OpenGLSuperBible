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
	CameraObject( GEvec3 position, GEvec3 rotation );
	~CameraObject();


	// Functions
	virtual std::string getClassName();
	glm::mat4 GetViewMatrix();

};


#endif /* CAMERAOBJECT_H */