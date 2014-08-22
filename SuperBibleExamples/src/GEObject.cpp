

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
	this->setOrientation( vmath::vec3( 0.0f, 0.0f, 0.0f ) );
	this->setScale( vmath::vec3( 1.0f, 1.0f, 1.0f ) );
}

GEObject::GEObject( vmath::vec3 position, vmath::vec3 orientation, vmath::vec3 scale, std::string name )
{
	this->GenerateID();
	this->setName( name );
	this->setPosition( position );
	this->setOrientation( orientation );
	this->setScale( scale );
}

//setters
void GEObject::setName( const std::string name )
{
	this->setName( name );
}

void GEObject::setPosition(const vmath::vec3 position)
{
	this->position = position;
}

void GEObject::setOrientation(const vmath::vec3 orientation)
{
	this->orientation = orientation;
}

void GEObject::setScale(const vmath::vec3 scale)
{
	this->scale = scale;
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

vmath::vec3 GEObject::getOrientation() const
{
	return this->orientation;
}

vmath::vec3 GEObject::getScale() const
{
	return this->scale;
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
	// Very simple way to generate a random id.  TODO investigate more thorough way.  UUID?
	std::random_device rd;
	int randNum = rd();
	char tString[128];
	_itoa( randNum, tString, 10 );
	
	id = std::string( tString );
}