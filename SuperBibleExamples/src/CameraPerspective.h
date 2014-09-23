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
#include "GEController.h"
#include "GEProperty.h"

class CameraPerspective: public CameraObject
{
private:
	//float fov;
	GEPropertyf1 fov;

public:

	// Structors
	CameraPerspective();
	CameraPerspective( glm::vec3 position, glm::vec3 rotation, float fov );
	~CameraPerspective();

	// Setters
	void setFov( const float fov );

	// Getters
	float getBaseFov() const;
	float getFinalFov() const;

	// Functions
	virtual std::string getClassName();

	/**
		Update()
		@param gameTime - time (in seconds) passed since game began.
		@param deltaTime - time (in seconds) passed since last frame.
	*/
	virtual void Update( const double gameTime, const double deltaTime);

	virtual void addFOVController( GEControllerf1* fovController );
	virtual void removeFOVController( const unsigned int index );
	

};

#endif /* CAMERAPERSPECTIVE_H */