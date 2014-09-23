
#include <map>
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

	// Initialize the controller list... add base "static" controller to the as the first
	this->addPositionController ( new GEControllerv3() );
	this->addRotationController ( new GEControllerv3() );	
	this->addScaleController ( new GEControllerv3() );

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

	// Initialize the controller list... add base "static" controller as the first
	this->addPositionController ( new GEControllerv3() );
	this->addRotationController ( new GEControllerv3() );	
	this->addScaleController ( new GEControllerv3() );

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

glm::vec3 GEObject::getBasePosition() const
{
	return this->position.getBaseValue();
}

glm::vec3 GEObject::getBaseRotation() const
{
	return this->rotation.getBaseValue();
}

glm::vec3 GEObject::getBaseScale() const
{
	return this->scale.getBaseValue();
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
	glm::vec3 transformedPosition;
	glm::vec3 transformedRotation;
	glm::vec3 transformedScale;

	transformedPosition = getTransformedPosition();
	transformedRotation = getTransformedRotation();
	transformedScale = getTransformedScale();

	transformMatrix = glm::translate( glm::mat4(), transformedPosition ) * 
		glm::rotate( glm::mat4(), transformedRotation.z, glm::vec3( 0.0f, 0.0f, 1.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.y, glm::vec3( 0.0f, 1.0f, 0.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.x, glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
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

glm::vec3 GEObject::getTransformedPosition() const
{
	return position.getFinalValue();
}

glm::vec3 GEObject::getTransformedRotation() const
{
	return rotation.getFinalValue();
}

glm::vec3 GEObject::getTransformedScale() const
{
	return scale.getFinalValue();
}

void GEObject::addPositionController( GEControllerv3* positionController )
{
	position.addController( positionController, this );
}

void GEObject::addRotationController( GEControllerv3* rotationController )
{
	rotation.addController( rotationController, this );
}

void GEObject::addScaleController( GEControllerv3* scaleController)
{
	scale.addController( scaleController, this );
}

void GEObject::removePositionController( const unsigned int index )
{
	this->position.removeController( index );
}

void GEObject::removeRotationController( const unsigned int index )
{
	this->rotation.removeController( index );
}
void GEObject::removeScaleController( const unsigned int index)
{
	this->scale.removeController( index );
}

void GEObject::setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities)
{
	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
}