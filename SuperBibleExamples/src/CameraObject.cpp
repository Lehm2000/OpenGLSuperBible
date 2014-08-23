


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
	this->targeted = targeted;
	this->targetPosition = targetPosition;
}

CameraObject::~CameraObject()
{
}

// Setters
void CameraObject::setTargeted( const bool targeted )
{
	this->targeted = targeted;
}

void CameraObject::setTargetPosition( const vmath::vec3 targetPosition )
{
	this->targetPosition = targetPosition;
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