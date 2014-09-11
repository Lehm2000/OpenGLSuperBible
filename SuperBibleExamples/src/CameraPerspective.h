#ifndef CAMERAPERSPECTIVE_H
#define CAMERAPERSPECTIVE_H

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

class CameraPerspective: public CameraObject
{
private:
	float fov;

public:

	// Structors
	CameraPerspective();
	CameraPerspective( glm::vec3 position, glm::vec3 rotation, float fov );
	~CameraPerspective();

	// Setters
	void setFov( const float fov );

	// Getters
	float getFov() const;

	// Functions
	virtual std::string getClassName();
};

#endif /* CAMERAPERSPECTIVE_H */