
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "CameraObject.h"

// Structors
CameraObject::CameraObject()
{
	this->setVisible( false );
}

CameraObject::CameraObject( glm::vec3 position, glm::vec3 rotation )
	:GEObject( position, rotation, glm::vec3( 1.0f, 1.0f, 1.0f ) )
{
	this->setVisible( false );
}

CameraObject::~CameraObject()
{
}

// Functions
std::string CameraObject::getClassName()
{
	return "CameraObject";
}

glm::mat4 CameraObject::GetViewMatrix()
{
	glm::mat4 viewMatrix;

	/*
	if (targeted)
	{
		viewMatrix = glm::lookAt( getTransformedPosition(), getTargetPosition(), glm::vec3( 0.0f, 1.0f, 0.0f ) );  //Currently does not support camera tilt... TODO need to figure up vector.
	}
	else
	{*/
		// First guess at this matrix... probably could be more efficient.
		viewMatrix = glm::inverse( glm::translate( glm::mat4(), getTransformedPosition() ) * glm::rotate(glm::mat4(), getTransformedRotation()[1], glm::vec3(0,1,0) ) * glm::rotate(glm::mat4(), getTransformedRotation()[0], glm::vec3(1,0,0) ) );
	//}
	
	return viewMatrix;
}

void CameraObject::CalcTargetRotation()
{
	/*  TODO: Move to lookat controller
	if ( this->targeted )	// Verify that this camera is targeted.
	{
		// We need to calculate the x and y rotations.  The z rotation is left alone to allow tilt/roll.

		// Grab the existing rotation
		glm::vec3 newRot = getRotation();
		
		// Get the delta of the camera and target positions
		glm::vec3 deltaVec = getTargetPosition() - getPosition();

		//newRot = glm::atan2(deltaVec);
		
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
	*/
}