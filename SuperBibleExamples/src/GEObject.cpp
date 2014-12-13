
#include <map>
#include <random>
#include <stdlib.h>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GEObject.h"
#include "GEObjectContainer.h"
#include "TypeDefinitions.h"
#include "GESettingConstant.h"
//#include "vmath.h"

// structors
GEObject::GEObject()
{
	this->GenerateID();
	
	this->setPositionStart( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->setRotationStart( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->setScaleStart( GEvec3( 1.0f, 1.0f, 1.0f ) );

	// Initialize the controller list... add base "static" controller to the as the first
	this->addPositionController( new GEControllerv3() );
	this->addRotationController( new GEControllerv3() );	
	this->addScaleController( new GEControllerv3() );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?
	
	// setup the material with a constant GESetting class
	this->setMaterial( new GESettingConstant< std::string >() );
	this->setMaterialValue( "" );

}

GEObject::GEObject( const GEObject& source )
{
	this->GenerateID();

	this->position = source.position;
	this->rotation = source.rotation;
	this->scale = source.scale;

	this->setVisible( source.isVisible() );
	this->setMesh( source.getMesh() );

	this->material = source.material->clone();
	//this->setMaterialValue( source.getMaterial() );
}

GEObject::GEObject( GEvec3 position, GEvec3 rotation, GEvec3 scale )
{
	this->GenerateID();
	
	this->setPositionStart( position );
	this->setRotationStart( rotation );
	this->setScaleStart( scale );

	// Initialize the controller list... add base "static" controller as the first
	this->addPositionController( new GEControllerv3() );
	this->addRotationController( new GEControllerv3() );	
	this->addScaleController( new GEControllerv3() );

	this->setVisible( true );
	this->setMesh( "" );			// Have a default mesh?

	// setup the material with a constant GESetting class
	this->setMaterial( new GESettingConstant< std::string >() );
	this->setMaterialValue( "" );
}

GEObject::~GEObject()
{
	if( this->material != nullptr )
	{
		delete this->material;
		this->material = nullptr;
	}
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

void GEObject::setMaterial( const GESetting< std::string >* material )
{
	if (this->material != nullptr )
		delete this->material;

	this->material = material->clone();
}

void GEObject::setMaterialValue( const std::string materialValue )
{
	this->material->setValue( materialValue );
}

void GEObject::setMaterialValueList( const std::vector<std::string> valueList )
{
	this->material->setValueList( valueList );
}

/*
void GEObject::setGameEntities( const GEObjectContainer* gameEntities )
{
	this->gameEntities = gameEntities;
}
*/



// Getters
std::string GEObject::getID() const
{
	return this->id;
}



bool GEObject::isVisible() const
{
	return this->visible;
}

std::string GEObject::getMesh() const
{
	return this->mesh;
}

const GESetting< std::string >* GEObject::getMaterial( ) const
{
	return this->material;
}

std::string GEObject::getMaterialValue() const
{
	return this->material->getValue();
}

std::vector< std::string> GEObject::getMaterialValueList( ) const
{
	return this->material->getValueList();
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

void GEObject::addPositionController( GEControllerv3* controller )
{
	position.addController( controller );
}

void GEObject::removePositionController( const unsigned int index )
{
	position.removeController( index );
}

void GEObject::addRotationController( GEControllerv3* controller)
{
	rotation.addController( controller );
}

void GEObject::removeRotationController( const unsigned int index )
{
	rotation.removeController( index );
}

void GEObject::addScaleController( GEControllerv3* controller )
{
	scale.addController( controller );
}

void GEObject::removeScaleController( const unsigned int index )
{
	scale.removeController( index );
}


glm::mat4 GEObject::GetTransformMatrix( const GEObjectContainer* gameEntities ) const
{
	glm::mat4 transformMatrix;
	GEvec3 transformedPosition;
	GEvec3 transformedRotation;
	GEvec3 transformedScale;

	transformedPosition = position.getFinalValue( gameEntities );		// get value after controllers applied
	transformedRotation = rotation.getFinalValue( gameEntities );		// get value after controllers applied
	transformedScale = scale.getFinalValue( gameEntities );			// get value after controllers applied

	transformMatrix = glm::translate( glm::mat4(), transformedPosition ) * 
		glm::rotate( glm::mat4(), transformedRotation.z, GEvec3( 0.0f, 0.0f, 1.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.y, GEvec3( 0.0f, 1.0f, 0.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.x, GEvec3( 1.0f, 0.0f, 0.0f ) ) *
		glm::scale( glm::mat4(), transformedScale);

	return transformMatrix;
}

void GEObject::Update( const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime)
{
	// Let property controllers do their thing.

	position.Update( this, gameEntities, gameTime, deltaTime);
	rotation.Update( this, gameEntities, gameTime, deltaTime);
	scale.Update( this, gameEntities, gameTime, deltaTime);
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

/*
void GEObject::setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities)
{
	this->setGameEntities( gameEntities );

	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
}*/