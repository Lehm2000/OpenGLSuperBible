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
#include "TypeDefinitions.h"

class CameraPerspective: public CameraObject
{
private:
	//float fov;
	GEPropertyf1 fov;

public:

	// Structors
	CameraPerspective();
	CameraPerspective( GEvec3 position, GEvec3 rotation, float fov );
	CameraPerspective( const CameraPerspective& source );
	~CameraPerspective();

	// Setters
	void setFov( const float fov );

	// Getters
	float getBaseFov() const;
	float getFinalFov() const;

	GEPropertyf1* getFOV();

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
	
	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual CameraPerspective* clone() const;

	/**
		setControllerGameEntitiesPointer()
		Sets the pointer to the gameEntities so the controller can access the properties of other game entities.
		@ param gameEntities - pointer to the gameEntities
		@ return void
	*/
	virtual void setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities);

};

#endif /* CAMERAPERSPECTIVE_H */