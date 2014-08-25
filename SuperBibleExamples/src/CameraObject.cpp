


#include "CameraObject.h"

// Structors
CameraObject::CameraObject()
{
	this->targeted = false;
	this->targetPosition = vmath::vec3( 0.0f, 0.0f, 0.0f );
}

CameraObject::CameraObject( vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, bool targeted, vmath::vec3 targetPosition )
	:GEObject( position, rotation, scale )
{
	this->setTargeted( targeted );
	this->setTargetPosition( targetPosition );
}

CameraObject::~CameraObject()
{
}

// Setters
void CameraObject::setTargeted( const bool targeted )
{
	this->targeted = targeted;

	// If the camera is set to targeted calculate its new rotation.
	if ( targeted )
		CalcTargetRotation();
}

void CameraObject::setTargetPosition( const vmath::vec3 targetPosition )
{
	this->targetPosition = targetPosition;

	// If the camera is set to targeted calculate its new rotation.
	if ( this->targeted )
		CalcTargetRotation();
}

// Getters
bool CameraObject::isTargeted() const
{
	return targeted;
}

vmath::vec3 CameraObject::getTargetPosition() const
{
	return targetPosition;
}

// Functions
vmath::mat4 CameraObject::GetViewMatrix()
{
	vmath::mat4 viewMatrix;
	
	return viewMatrix;
}

void CameraObject::CalcTargetRotation()
{
	if ( this->targeted )	// Verify that this camera is targeted.
	{
		// We need to calculate the x and y rotations.  The z rotation is left alone to allow tilt/roll.

		// Grab the existing rotation
		vmath::vec3 newRot = getRotation();
		
		// Get the delta of the camera and target positions
		vmath::vec3 deltaVec = getTargetPosition() - getPosition();
		
		// first x (pitch) rotation
		if ( deltaVec[1] == 0 && deltaVec[2] == 0 )  // If points are the same.
		{
			newRot[0] = 0.0f;
		}
		else
		{
			newRot[0] = atan2( deltaVec[2], deltaVec[1] );
		}

		// now y (yaw) rotation
		if ( deltaVec[0] == 0 && deltaVec[2] == 0 )  // If points are the same.
		{
			newRot[1] = 0.0f;
		}
		else
		{
			newRot[1] = atan2( deltaVec[2], deltaVec[0] );
		}

		// We don't touch the z.
		
		// assign the new calculated rotation back.
		setRotation ( newRot );
	}
}