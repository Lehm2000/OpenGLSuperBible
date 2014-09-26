
#include <map>
#include <random>
#include <stdlib.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GEObject.h"
#include "GEObjectContainer.h"
#include "TypeDefinitions.h"
//#include "vmath.h"

// structors
GEObject::GEObject()
{
	this->GenerateID();
	this->setName( "" );
	
	this->getPosition()->setValue( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->getRotation()->setValue( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->getScale()->setValue( GEvec3( 1.0f, 1.0f, 1.0f ) );

	// Initialize the controller list... add base "static" controller to the as the first
	this->getPosition()->addController( new GEControllerv3(), this );
	this->getRotation()->addController( new GEControllerv3(), this );	
	this->getScale()->addController( new GEControllerv3(), this );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::GEObject( const GEObject& source )
{
	this->GenerateID();
	this->setName( source.getName() );

	this->position = source.position;
	this->rotation = source.rotation;
	this->scale = source.scale;

	this->setVisible( source.isVisible() );
	this->setMesh( source.getMesh() );
	this->setMaterial( source.getMaterial() );
}

GEObject::GEObject( GEvec3 position, GEvec3 rotation, GEvec3 scale, std::string name )
{
	this->GenerateID();
	this->setName( name );
	
	this->getPosition()->setValue( position );
	this->getRotation()->setValue( rotation );
	this->getScale()->setValue( scale );

	// Initialize the controller list... add base "static" controller as the first
	this->getPosition()->addController( new GEControllerv3(), this );
	this->getRotation()->addController( new GEControllerv3(), this );	
	this->getScale()->addController( new GEControllerv3(), this );

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

/*void GEObject::setPosition( const GEvec3 position )
{
	this->position.setValue( position);
}

void GEObject::setRotation( const GEvec3 rotation )
{
	this->rotation.setValue( rotation );
}

void GEObject::setScale(const GEvec3 scale)
{
	this->scale.setValue( scale );
}*/

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

/*
GEvec3 GEObject::getBasePosition() const
{
	return this->position.getBaseValue();
}

GEvec3 GEObject::getBaseRotation() const
{
	return this->rotation.getBaseValue();
}

GEvec3 GEObject::getBaseScale() const
{
	return this->scale.getBaseValue();
}
*/

GEPropertyv3* GEObject::getPosition()
{
	return &position;
}

GEPropertyv3* GEObject::getRotation()
{
	return &this->rotation;
}

GEPropertyv3* GEObject::getScale()
{
	return &this->scale;
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
	GEvec3 transformedPosition;
	GEvec3 transformedRotation;
	GEvec3 transformedScale;

	transformedPosition = position.getFinalValue();		// get value after controllers applied
	transformedRotation = rotation.getFinalValue();		// get value after controllers applied
	transformedScale = scale.getFinalValue();			// get value after controllers applied

	transformMatrix = glm::translate( glm::mat4(), transformedPosition ) * 
		glm::rotate( glm::mat4(), transformedRotation.z, GEvec3( 0.0f, 0.0f, 1.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.y, GEvec3( 0.0f, 1.0f, 0.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.x, GEvec3( 1.0f, 0.0f, 0.0f ) ) *
		glm::scale( glm::mat4(), transformedScale);

	return transformMatrix;
}

void GEObject::Update( const double gameTime, const double deltaTime)
{
	// Let property controllers do their thing.

	position.Update( gameTime, deltaTime);
	rotation.Update( gameTime, deltaTime);
	scale.Update( gameTime, deltaTime);
}

GEObject* GEObject::clone() const
{
	return new GEObject(*this);
}

void GEObject::setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities)
{
	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
}