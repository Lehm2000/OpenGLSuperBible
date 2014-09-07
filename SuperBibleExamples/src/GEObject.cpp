

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

	GEController* tempController = new GEController(); 
	this->setPositionController ( tempController );
	
	tempController = new GEController(); 
	this->setRotationController ( tempController );
	
	tempController = new GEController(); 
	this->setScaleController ( tempController );

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

	GEController* tempController = new GEController(); 
	this->setPositionController ( tempController );
	
	tempController = new GEController(); 
	this->setRotationController ( tempController );
	
	tempController = new GEController(); 
	this->setScaleController ( tempController );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	this->setMaterial( "" );		// Have a default material?
}

GEObject::~GEObject()
{
	if (this->positionController != nullptr )
	{
		delete positionController;
		positionController = 0;
	}

	if (this->rotationController != nullptr )
	{
		delete rotationController;
		rotationController = 0;
	}

	if (this->scaleController != nullptr )
	{
		delete scaleController;
		scaleController = 0;
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

void GEObject::setPositionController( GEController* positionController )
{
	if (this->positionController != nullptr )
	{
		delete this->positionController;
		this->positionController = 0;
	}

	this->positionController = positionController;
}

void GEObject::setRotationController( GEController* rotationController )
{
	if (this->rotationController != nullptr )
	{
		delete this->rotationController;
		this->rotationController = 0;
	}

	this->rotationController = rotationController;
}

void GEObject::setScaleController( GEController* scaleController)
{
	if (this->scaleController != nullptr )
	{
		delete this->scaleController;
		this->scaleController = 0;
	}

	this->scaleController = scaleController;
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

const GEController* GEObject::getPositionController() const
{
	return this->positionController;
}

const GEController* GEObject::getRotationController() const
{
	return this->rotationController;
}

const GEController* GEObject::getScaleController() const
{
	return this->scaleController;
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

	glm::vec3 combinedPosition = getPosition() ;  // place to hold the combined position information from the position controllers. (Once we enable more than 1)
	glm::vec3 combinedRotation = getRotation() ;  // place to hold the combined position information from the position controllers. (Once we enable more than 1)
	glm::vec3 combinedScale = getScale() ;  // place to hold the combined position information from the position controllers. (Once we enable more than 1)

	// Calculate transforms (eventually loop through the controllers here)
	combinedPosition = positionController->CalcTransform( combinedPosition );
	combinedRotation = rotationController->CalcTransform( combinedRotation );
	combinedScale = scaleController->CalcTransform( combinedScale );

	transformMatrix = glm::translate( glm::mat4(), combinedPosition ) * 
		glm::rotate( glm::mat4(), combinedRotation[2], glm::vec3( 0.0f, 0.0f, 1.0f ) ) * 
		glm::rotate( glm::mat4(), combinedRotation[1], glm::vec3( 0.0f, 1.0f, 0.0f ) ) * 
		glm::rotate( glm::mat4(), combinedRotation[0], glm::vec3( 1.0f, 0.0f, 0.0f ) ) *
		glm::scale( glm::mat4(), combinedScale);

	return transformMatrix;
}

void GEObject::Update( const double gameTime, const double deltaTime)
{
	// Let object controllers do their thing.
	this->positionController->Control( this->getPosition(), gameTime, deltaTime );
	this->rotationController->Control( this->getRotation(), gameTime, deltaTime );
	this->scaleController->Control( this->getScale(), gameTime, deltaTime );
}