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

#include "GEObject.h"
#include "vmath.h"

#define CAMTYPE_PERSPECTIVE				1
#define CAMTYPE_PERSPECTIVE_TARGETED	2
#define CAMTYPE_ORTHO					3
#define CAMTYPE_ORTHO_TARGETED			4
#define CAMTYPE_2D						5

class CameraObject: public GEObject
{
private:
	// Members
	
	// Target info
	bool targeted;
	vmath::vec3 targetPosition;
	

public:
	// Structors
	CameraObject();
	CameraObject( vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, bool targeted, vmath::vec3 targetPosition = vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	~CameraObject();

	// Setters
	void setTargeted( const bool isTargeted );
	void setTargetPosition( const vmath::vec3 targetPosition );

	// Getters
	bool isTargeted() const;
	vmath::vec3 getTargetPosition() const;
};


#endif /* CAMERAOBJECT_H */