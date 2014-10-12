
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
std::string CameraObject::getClassName() const
{
	return "CameraObject";
}

glm::mat4 CameraObject::GetViewMatrix() const
{
	glm::mat4 viewMatrix;

	// First guess at this matrix... probably could be more efficient.
	viewMatrix = glm::inverse( glm::translate( glm::mat4(), this->getPositionFinal() ) * glm::rotate(glm::mat4(), this->getRotationFinal()[1], GEvec3(0,1,0) ) * glm::rotate(glm::mat4(), this->getRotationFinal()[0], GEvec3(1,0,0) ) );
	
	return viewMatrix;
}

CameraObject* CameraObject::clone() const
{
	return new CameraObject( *this );
}