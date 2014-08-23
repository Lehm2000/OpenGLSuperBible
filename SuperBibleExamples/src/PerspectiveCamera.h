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

#include "CameraObject.h"

class PerspectiveCamera: public CameraObject
{
private:
	float fov;

public:

	// Structors
	PerspectiveCamera();
	PerspectiveCamera( vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, float fov, bool targeted, vmath::vec3 targetPosition = vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	~PerspectiveCamera();

	// Setters
	void setFov( const float fov );

	// Getters
	float getFov() const;
};

#endif /* PERSPECTIVECAMERA_H */