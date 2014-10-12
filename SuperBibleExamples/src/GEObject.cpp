
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
	
	this->setPositionStart( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotationStart( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->setScaleStart( GEvec3( 1.0f, 1.0f, 1.0f ) );

	// Initialize the controller list... add base "static" controller to the as the first
	this->addPositionController( new GEControllerv3(), this );
	this->addRotationController( new GEControllerv3(), this );	
	this->addScaleController( new GEControllerv3(), this );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::GEObject( const GEObject& source )
{
	this->GenerateID();

	this->position = source.position;
	this->rotation = source.rotation;
	this->scale = source.scale;

	this->setVisible( source.isVisible() );
	this->setMesh( source.getMesh() );
	this->setMaterial( source.getMaterial() );
}

GEObject::GEObject( GEvec3 position, GEvec3 rotation, GEvec3 scale )
{
	this->GenerateID();
	
	this->setPositionStart( position );
	this->setRotationStart( rotation );
	this->setScaleStart( scale );

	// Initialize the controller list... add base "static" controller as the first
	this->addPositionController( new GEControllerv3(), this );
	this->addRotationController( new GEControllerv3(), this );	
	this->addScaleController( new GEControllerv3(), this );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::~GEObject()
{
	
}

// Setters

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

void GEObject::setGameEntities( const GEObjectContainer* gameEntities )
{
	this->gameEntities = gameEntities;
}



// Getters
std::string GEObject::getID() const
{
	return this->id;
}

	/*
GEPropertyv3* GEObject::getPosition()
{
	return &this->position;
}

const GEPropertyv3* GEObject::getPosition() const
{
	return &this->position;
}

GEPropertyv3* GEObject::getRotation()
{
	return &this->rotation;
}

const GEPropertyv3* GEObject::getRotation() const
{
	return &this->rotation;
}

GEPropertyv3* GEObject::getScale()
{
	return &this->scale;
}

const GEPropertyv3* GEObject::getScale() const
{
	return &this->scale;
}*/

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

std::string GEObject::getClassName() const
{
	return "GEObject";
}

void GEObject::addPositionController( GEControllerv3* controller, const GEObject* parent )
{
	position.addController( controller, parent );
}

void GEObject::removePositionController( const unsigned int index )
{
	position.removeController( index );
}

void GEObject::addRotationController( GEControllerv3* controller, const GEObject* parent )
{
	rotation.addController( controller, parent );
}

void GEObject::removeRotationController( const unsigned int index )
{
	rotation.removeController( index );
}

void GEObject::addScaleController( GEControllerv3* controller, const GEObject* parent )
{
	scale.addController( controller, parent );
}

void GEObject::removeScaleController( const unsigned int index )
{
	scale.removeController( index );
}


glm::mat4 GEObject::GetTransformMatrix() const
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

void GEObject::ProcessInput( const GEInputState* inputState)
{
	// pass it on to the controllers to do their thing.
	position.ProcessInput( inputState );
	rotation.ProcessInput( inputState );
	scale.ProcessInput( inputState );
}

GEObject* GEObject::clone() const
{
	return new GEObject(*this);
}

void GEObject::setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities)
{
	this->setGameEntities( gameEntities );

	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
}