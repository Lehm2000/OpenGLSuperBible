#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

/**
	PerspectiveCamera
	Purpose: Defines a type of camera for perspective. Extends CameraObject.
	Author: Jeff Adams

	Updates:
	2014-08-23
	Initial Version
*/

#include <glm\glm.hpp>

#include "CameraObject.h"

class PerspectiveCamera: public CameraObject
{
private:
	float fov;

public:

	// Structors
	PerspectiveCamera();
	PerspectiveCamera( glm::vec3 position, glm::vec3 rotation, float fov, bool targeted, glm::vec3 targetPosition = glm::vec3( 0.0f, 0.0f, 0.0f ) );
	~PerspectiveCamera();

	// Setters
	void setFov( const float fov );

	// Getters
	float getFov() const;

	// Functions
	virtual std::string getClassName();
};

#endif /* PERSPECTIVECAMERA_H */