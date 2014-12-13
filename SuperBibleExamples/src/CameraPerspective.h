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

	void setFovStart( const float fov ) { this->fov.setValue( fov ); };
	void setFovMin( const float min ) { this->fov.setMin( min ); };
	void setFovUseMin( const bool useMin ) { this->fov.setUseMin( useMin ); };
	void setFovMax( const float max ) { this->fov.setMax( max ); };
	void setFovUseMax( const bool useMax ) { this->fov.setUseMax( useMax ); };

	

	// Getters
	float getFovStart() const { return this->fov.getBaseValue(); };
	float getFovFinal( const GEObjectContainer* gameEntities ) const { return this->fov.getFinalValue( gameEntities ); };
	float getFovnMin() const { return this->fov.getMinValue(); };
	bool getFovUseMin() const { return this->fov.getUseMin(); };
	float getFovMax() const { return this->fov.getMaxValue(); };
	bool getFovUseMax() const { return this->fov.getUseMax(); };

	//GEPropertyf1* getFOVProp();
	//const GEPropertyf1* getFOVProp() const;

	// Functions
	virtual std::string getClassName() const;

	/**
		Update()
		@param gameTime - time (in seconds) passed since game began.
		@param deltaTime - time (in seconds) passed since last frame.
	*/
	virtual void Update(  const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime);

	virtual void addFovController( GEControllerf1* fovController );
	virtual void removeFovController( const unsigned int index );
	
	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual CameraPerspective* clone() const;
	
	/**
		ProcessInput
		Function for processing input from the user.  Meant to be stored in the
		inputFunction list as a pointer.  Takes the input state and passes it
		to the Controllable Properties.
	*/
	virtual void ProcessInput( const GEInputState* inputState );

	/**
		setControllerGameEntitiesPointer()
		Sets the pointer to the gameEntities so the controller can access the properties of other game entities.
		@ param gameEntities - pointer to the gameEntities
		@ return void
	*/
	//virtual void setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities);

};

#endif /* CAMERAPERSPECTIVE_H */