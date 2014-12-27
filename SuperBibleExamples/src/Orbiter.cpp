#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "Orbiter.h"



// Structors

Orbiter::Orbiter()
	:GEObject()
{
	this->setOrbitTargetName( "" );
	this->setOrbitAngleStart( GEvec3( 0.0f, 0.0f, 0.0f ) );
	this->setOrbitDistanceStart( 0.0f );

	// Initialize the controller list... add base "static" controller to the as the first
	this->addOrbitAngleController( new GEControllerv3() );
	this->addOrbitDistanceController( new GEControllerf1() );	
}

Orbiter::Orbiter( GEvec3 position, GEvec3 rotation, GEvec3 scale, std::string orbitTargetName, GEvec3 orbitAngle, float orbitDistance )
	:GEObject( position, rotation, scale )
{
	this->setOrbitTargetName( orbitTargetName );
	this->setOrbitAngleStart( orbitAngle );
	this->setOrbitDistanceStart( orbitDistance );

	// Initialize the controller list... add base "static" controller to the as the first
	this->addOrbitAngleController( new GEControllerv3() );
	this->addOrbitDistanceController( new GEControllerf1() );	
}

Orbiter::Orbiter( const Orbiter& source )
	:GEObject( source )
{
	this->setOrbitTargetName( source.orbitTargetName );
	this->orbitAngle = source.orbitAngle;
	this->orbitDistance = source.orbitDistance;
}

Orbiter::~Orbiter()
{
}


// Setters

void Orbiter::setOrbitTargetName( const std::string orbitTargetName )
{
	this->orbitTargetName = orbitTargetName;
}

	
// Getters

std::string Orbiter::getOrbitTargetName() const
{
	return orbitTargetName;
}


std::string Orbiter::getClassName() const
{
	return "Orbiter";
}

void Orbiter::addOrbitAngleController( GEControllerv3* controller )
{
	this->orbitAngle.addController( controller );
}

void Orbiter::removeOrbitAngleController( const unsigned int index )
{
	this->orbitAngle.removeController( index );
}

void Orbiter::addOrbitDistanceController( GEControllerf1* controller )
{
	this->orbitDistance.addController( controller );
}

void Orbiter::removeOrbitDistanceController( const unsigned int index )
{
	this->orbitDistance.removeController( index );
}

GEvec3 Orbiter::GetOrbitPosition( const GEObjectContainer* gameEntities ) const
{
	GEvec3 orbitPosition;
	const GEObject* targetObject = gameEntities->GetObject( this->orbitTargetName );
	
	if ( targetObject != nullptr )
	{
		const GEvec3 targetPos = targetObject->getPositionFinal( gameEntities );
		
		// Create a new starting vector based on the distance.
		glm::vec4 distVector( 0.0f, 0.0f, orbitDistance.getFinalValue( gameEntities ), 1.0f );

		// create a rotation matrix and rotate the distVector by it.
		glm::vec4 newPos = glm::rotate( glm::mat4(), orbitAngle.getFinalValue( gameEntities ).z, glm::vec3( 0.0f, 0.0f, 1.0f ) ) * glm::rotate( glm::mat4(), orbitAngle.getFinalValue( gameEntities ).y, glm::vec3( 0.0f, 1.0f, 0.0f ) ) * glm::rotate( glm::mat4(), orbitAngle.getFinalValue( gameEntities ).x, glm::vec3( 1.0f, 0.0f, 0.0f ) ) * distVector;
		
		// next transform the position to target.
		orbitPosition = targetPos + glm::vec3( newPos.x, newPos.y, newPos.z );

		// assign the new calculated rotation back... this ignores what we were given.
		//transformedValue = newPos;
	}
	else
	{
		// just return the objects base position
		orbitPosition = this->getPositionStart();
	}
	return orbitPosition;
}

glm::mat4 Orbiter::GetTransformMatrix( const GEObjectContainer* gameEntities ) const
{
	glm::mat4 transformMatrix;
	GEvec3 transformedPosition;
	GEvec3 transformedRotation;
	GEvec3 transformedScale;

	transformedPosition = this->GetOrbitPosition( gameEntities );								// calculated based on orbit distance and orbit angle
	transformedRotation = rotation.getFinalValue( gameEntities );		// get value after controllers applied
	transformedScale = scale.getFinalValue( gameEntities );			// get value after controllers applied

	transformMatrix = glm::translate( glm::mat4(), transformedPosition ) * 
		glm::rotate( glm::mat4(), transformedRotation.z, GEvec3( 0.0f, 0.0f, 1.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.y, GEvec3( 0.0f, 1.0f, 0.0f ) ) * 
		glm::rotate( glm::mat4(), transformedRotation.x, GEvec3( 1.0f, 0.0f, 0.0f ) ) *
		glm::scale( glm::mat4(), transformedScale);

	return transformMatrix;
}

void Orbiter::Update( const GEObjectContainer* gameEntities, const double gameTime, const double deltaTime)
{
	// Let property controllers do their thing.

	GEObject::Update( gameEntities, gameTime, deltaTime ); // call the parent update function.

	orbitAngle.Update( this, gameEntities, gameTime, deltaTime );
	orbitDistance.Update( this, gameEntities, gameTime, deltaTime );
}

	
void Orbiter::ProcessInput( const GEInputState* inputState )
{
	// pass it on to the controllers to do their thing.

	GEObject::ProcessInput( inputState );

	orbitAngle.ProcessInput( inputState );
	orbitDistance.ProcessInput( inputState );
}

	
GEObject* Orbiter::clone() const
{
	return new Orbiter(*this);
}

/*
void Orbiter::setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities)
{
	this->setGameEntities( gameEntities );

	// give all the transform controllers a pointer to the gameEntities

	position.setControllerGameEntitiesPointer( gameEntities );
	rotation.setControllerGameEntitiesPointer( gameEntities );
	scale.setControllerGameEntitiesPointer( gameEntities );
	orbitAngle.setControllerGameEntitiesPointer( gameEntities );
	orbitDistance.setControllerGameEntitiesPointer( gameEntities );
}*/
