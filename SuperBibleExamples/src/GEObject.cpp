

#include <random>
#include <stdlib.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GEObject.h"
//#include "vmath.h"

// structors
GEObject::GEObject()
{
	this->GenerateID();
	this->setName( "" );
	
	this->setPosition( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotation( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScale( glm::vec3( 1.0f, 1.0f, 1.0f ) );

	this->setPositionVel( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotationVel( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScaleVel( glm::vec3( 0.0f, 0.0f, 0.0f ) );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::GEObject( glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string name )
{
	this->GenerateID();
	this->setName( name );
	
	this->setPosition( position );
	this->setRotation( rotation );
	this->setScale( scale );

	this->setPositionVel( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotationVel( glm::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScaleVel( glm::vec3( 0.0f, 0.0f, 0.0f ) );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::~GEObject()
{
}

//setters
void GEObject::setName( const std::string name )
{
	this->name = name;
}

void GEObject::setPosition( const glm::vec3 position )
{
	this->position = position;
}

void GEObject::setRotation( const glm::vec3 rotation )
{
	this->rotation = rotation;
}

void GEObject::setScale(const glm::vec3 scale)
{
	this->scale = scale;
}

void GEObject::setPositionVel(const glm::vec3 positionVel)
{
	this->positionVel = positionVel;
}

void GEObject::setRotationVel(const glm::vec3 rotationVel)
{
	this->rotationVel = rotationVel;
}

void GEObject::setScaleVel(const glm::vec3 scaleVel)
{
	this->scaleVel = scaleVel;
}

void GEObject::setVisible( const bool visible )
{
	this->visible = visible;
}

void GEObject::setMesh( const std::string mesh )
{
	this->mesh = mesh;
}

void GEObject::setMaterial( const std::string material )
{
	this->material = material;
}

//getters
std::string GEObject::getID() const
{
	return this->id;
}

std::string GEObject::getName() const
{
	return this->name;
}

glm::vec3 GEObject::getPosition() const
{
	return this->position;
}

glm::vec3 GEObject::getRotation() const
{
	return this->rotation;
}

glm::vec3 GEObject::getScale() const
{
	return this->scale;
}

glm::vec3 GEObject::getPositionVel() const
{
	return this->positionVel;
}

glm::vec3 GEObject::getRotationVel() const
{
	return this->rotationVel;
}

glm::vec3 GEObject::getScaleVel() const
{
	return this->scaleVel;
}

bool GEObject::isVisible() const
{
	return this->visible;
}

std::string GEObject::getMesh() const
{
	return this->mesh;
}

std::string GEObject::getMaterial() const
{
	return this->material;
}

// Comparison
bool GEObject::operator==( const GEObject& other ) const
{
	return this->id == other.id;
}

bool GEObject::operator!=( const GEObject& other ) const
{
	return this->id != other.id;
}

bool GEObject::operator<( const GEObject& other ) const
{
	return this->id < other.id;
}

bool GEObject::operator>( const GEObject& other ) const
{
	return this->id > other.id;
}

bool GEObject::operator<=( const GEObject& other ) const
{
	return this->id <= other.id;
}

bool GEObject::operator>=( const GEObject& other ) const
{
	return this->id >= other.id;
}


// Functions
void GEObject::GenerateID()
{
	// Very simple way to generate a random id.  TODO investigate more thorough way to guaruntee uniqueness.  UUID?
	std::random_device rd;
	int randNum = rd();
	char tString[128];
	_itoa( randNum, tString, 10 );
	
	id = std::string( tString );
}

std::string GEObject::getClassName()
{
	return "GEObject";
}

glm::mat4 GEObject::GetTransformMatrix()
{
	glm::mat4 transformMatrix;

	transformMatrix = glm::translate( glm::mat4(), getPosition() ) * glm::rotate( glm::mat4(), getRotation()[2], glm::vec3( 0.0f, 0.0f, 1.0f ) ) * glm::rotate( glm::mat4(), getRotation()[1], glm::vec3( 0.0f, 1.0f, 0.0f ) ) * glm::rotate( glm::mat4(), getRotation()[0], glm::vec3( 1.0f, 0.0f, 0.0f ) );

	return transformMatrix;
}

void GEObject::Update(const double deltaTime)
{
	// Apply velocities
	this->rotation = this->rotation + ( this->rotationVel * (float)deltaTime );
}