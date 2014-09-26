
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "TypeDefinitions.h"
#include "CameraObject.h"

// Structors
CameraObject::CameraObject()
{
	this->setVisible( false );
}

CameraObject::CameraObject( GEvec3 position, GEvec3 rotation )
	:GEObject( position, rotation, GEvec3( 1.0f, 1.0f, 1.0f ) )
{
	this->setVisible( false );
}

CameraObject::CameraObject( const CameraObject& source )
	:GEObject( source )
{

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
		viewMatrix = glm::lookAt( getTransformedPosition(), getTargetPosition(), GEvec3( 0.0f, 1.0f, 0.0f ) );  //Currently does not support camera tilt... TODO need to figure up vector.
	}
	else
	{*/
		// First guess at this matrix... probably could be more efficient.
	viewMatrix = glm::inverse( glm::translate( glm::mat4(), getPosition()->getFinalValue() ) * glm::rotate(glm::mat4(), getRotation()->getFinalValue()[1], GEvec3(0,1,0) ) * glm::rotate(glm::mat4(), getRotation()->getFinalValue()[0], GEvec3(1,0,0) ) );
	//}
	
	return viewMatrix;
}

CameraObject* CameraObject::clone() const
{
	return new CameraObject( *this );
}