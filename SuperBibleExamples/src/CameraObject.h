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
//#include "vmath.h"

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
	glm::vec3 targetPosition;
	

public:
	// Structors
	CameraObject();
	CameraObject( glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, bool targeted, glm::vec3 targetPosition = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	~CameraObject();

	// Setters
	void setTargeted( const bool isTargeted );
	void setTargetPosition( const glm::vec3 targetPosition );

	// Getters
	bool isTargeted() const;
	glm::vec3 getTargetPosition() const;

	// Functions
	virtual std::string getClassName();
	glm::mat4 GetViewMatrix();
	void CalcTargetRotation();
};


#endif /* CAMERAOBJECT_H */