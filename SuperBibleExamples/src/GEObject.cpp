

#include <random>
#include <stdlib.h>

#include "GEObject.h"
#include "vmath.h"

// structors
GEObject::GEObject()
{
	this->GenerateID();
	this->setName( "" );
	
	this->setPosition( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotation( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScale( vmath::vec3( 1.0f, 1.0f, 1.0f ) );

	this->setPositionVel( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotationVel( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScaleVel( vmath::vec3( 0.0f, 0.0f, 0.0f ) );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::GEObject( vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, std::string name )
{
	this->GenerateID();
	this->setName( name );
	
	this->setPosition( position );
	this->setRotation( rotation );
	this->setScale( scale );

	this->setPositionVel( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotationVel( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScaleVel( vmath::vec3( 0.0f, 0.0f, 0.0f ) );

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

void GEObject::setPosition( const vmath::vec3 position )
{
	this->position = position;
}

void GEObject::setRotation( const vmath::vec3 rotation )
{
	this->rotation = rotation;
}

void GEObject::setScale(const vmath::vec3 scale)
{
	this->scale = scale;
}

void GEObject::setPositionVel(const vmath::vec3 positionVel)
{
	this->positionVel = positionVel;
}

void GEObject::setRotationVel(const vmath::vec3 rotationVel)
{
	this->rotationVel = rotationVel;
}

void GEObject::setScaleVel(const vmath::vec3 scaleVel)
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

vmath::vec3 GEObject::getPosition() const
{
	return this->position;
}

vmath::vec3 GEObject::getRotation() const
{
	return this->rotation;
}

vmath::vec3 GEObject::getScale() const
{
	return this->scale;
}

vmath::vec3 GEObject::getPositionVel() const
{
	return this->positionVel;
}

vmath::vec3 GEObject::getRotationVel() const
{
	return this->rotationVel;
}

vmath::vec3 GEObject::getScaleVel() const
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

vmath::mat4 GEObject::GetTransformMatrix()
{
	vmath::mat4 transformMatrix;

	return transformMatrix;
}