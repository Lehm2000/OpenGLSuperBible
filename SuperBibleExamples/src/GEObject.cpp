
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
	this->addPositionController ( new GEController() );
	this->addRotationController ( new GEController() );	
	this->addScaleController ( new GEController() );

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
	this->addPositionController ( new GEController() );
	this->addRotationController ( new GEController() );	
	this->addScaleController ( new GEController() );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::~GEObject()
{
	// Delete all the controllers
	for ( unsigned int i = 0; i < positionControllers.size(); i++)
	{
		delete positionControllers[i];
		positionControllers[i] = nullptr;
	}

	for ( unsigned int i = 0; i < rotationControllers.size(); i++)
	{
		delete rotationControllers[i];
		rotationControllers[i] = nullptr;
	}

	for ( unsigned int i = 0; i < scaleControllers.size(); i++)
	{
		delete scaleControllers[i];
		scaleControllers[i] = nullptr;
	}
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
	// Let object controllers do their thing.

	for ( unsigned int i = 0; i < positionControllers.size(); i++)
		positionControllers[i]->Control( this->getPosition(), gameTime, deltaTime );

	for ( unsigned int i = 0; i < rotationControllers.size(); i++)
		rotationControllers[i]->Control( this->getRotation(), gameTime, deltaTime );

	for ( unsigned int i = 0; i < scaleControllers.size(); i++)
		scaleControllers[i]->Control( this->getScale(), gameTime, deltaTime );

}

glm::vec3 GEObject::getTransformedPosition() const
{
	glm::vec3 combinedPosition = getPosition() ;  // place to hold the combined position information from the position controllers.

	for ( unsigned int i = 0; i < positionControllers.size(); i++)
		combinedPosition = positionControllers[i]->CalcTransform( combinedPosition );

	return combinedPosition;
}

glm::vec3 GEObject::getTransformedRotation() const
{
	glm::vec3 combinedRotation = getRotation() ;  // place to hold the combined position information from the position controllers.

	for ( unsigned int i = 0; i < rotationControllers.size(); i++)
		combinedRotation = rotationControllers[i]->CalcTransform( combinedRotation );

	return combinedRotation;
}

glm::vec3 GEObject::getTransformedScale() const
{
	glm::vec3 combinedScale = getScale() ;  // place to hold the combined position information from the position controllers.

	for ( unsigned int i = 0; i < scaleControllers.size(); i++)
		combinedScale = scaleControllers[i]->CalcTransform( combinedScale );

	return combinedScale;
}

void GEObject::addPositionController( GEController* positionController )
{
	positionController->setParent( this );
	this->positionControllers.push_back( positionController->clone() );  // add a copy of the controller to the controller vector
}

void GEObject::addRotationController( GEController* rotationController )
{
	rotationController->setParent( this );
	this->rotationControllers.push_back( rotationController->clone() );		// add a copy of the controller to the controller vector
}

void GEObject::addScaleController( GEController* scaleController)
{
	scaleController->setParent( this );
	this->scaleControllers.push_back( scaleController->clone() );	// add a copy of the controller to the controller vector
}

void GEObject::removePositionController( const unsigned int index )
{
	this->positionControllers.erase( positionControllers.begin() + index );
}

void GEObject::removeRotationController( const unsigned int index )
{
	this->rotationControllers.erase( rotationControllers.begin() + index );
}
void GEObject::removeScaleController( const unsigned int index)
{
	this->scaleControllers.erase( scaleControllers.begin() + index );
}

void GEObject::setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities)
{
	// give all the transform controllers a pointer to the gameEntities

	for ( unsigned int i = 0; i < positionControllers.size(); i++)
		positionControllers[i]->setGameEntities( gameEntities );

	for ( unsigned int i = 0; i < rotationControllers.size(); i++)
		rotationControllers[i]->setGameEntities( gameEntities );

	for ( unsigned int i = 0; i < scaleControllers.size(); i++)
		scaleControllers[i]->setGameEntities( gameEntities );
}