#ifndef GECONTROLLER_H
#define GECONTROLLER_H

/**
	GEController Class
	
	Purpose: 
	This is the base controller class for objects in the game.
	Controllers are the means by which objects are moved, rotated and scaled.
	The base class merely returns what was passed to it.  The object it is
	attached to basically is static then.


	Author: Jeff Adams
*/

#include <map>
#include <glm\glm.hpp>

#include "GEObject.h"

class GEObject;

template <class T>
class GEController
{
protected:
	// Members
	
	const GEObject* parent;  // in case the controller needs to access the properties of the parent.  Like the lookat controller needs to know the parents position for example.
	const std::map< std::string, GEObject* >* gameEntities;  // pointer to the master gameEntity list.  In case the controller needs to know the properties of some other object in the world.  This once scares me a bit... I know its const... but is there a better way to get this info?

	T transformedValue;	// the transformed data.

public:
	// Structors
	
	GEController();
	GEController( const GEObject* parent, const std::map< std::string, GEObject* >* gameEntities );
	GEController( const GEController& source);
	virtual ~GEController();

	// Setters

	virtual void setParent( const GEObject* parent );
	virtual void setGameEntities( const std::map< std::string, GEObject* >* gameEntities );

	// Getters

	virtual const GEObject* getParent() const;
	virtual const std::map< std::string, GEObject* >* getGameEntities() const;

	// Functions 

	/**
		clone()
		Returns a pointer to a copy of itself.  Used when you need a copy of a
		derived class and only have a pointer to the base class.
		@return - pointer to a copy of this object
	*/
	virtual GEController<T>* clone() const;

	/**
		Control()
		Takes objectVector and passed it on without modification.
		@param startVector - starting point of the object.  Could be position, rotation or scale
		@param gameTime - time since the game started
		@param deltaTime - time since the last frame
		@return
	*/
	virtual void Control( T initialValue, double gameTime, double deltaTime );

	/**
		CalcTransform()
		Takes a source vector and combines it with the transformedVector
		@param sourceVector - vector to be combined with the controllers transformedVector.
			Usually the objects original transform.
	*/
	virtual T CalcTransform( T sourceValue ); 
};

// Structors
template <class T>
GEController<T>::GEController()
{
	//this->transformedVector = glm::vec3( 0.0f, 0.0f, 0.0f );

	this->setParent( nullptr );
	this->setGameEntities( nullptr );
}

template <class T>
GEController<T>::GEController( const GEObject* parent, const std::map< std::string, GEObject* >* gameEntities )
{
	//this->transformedVector = glm::vec3( 0.0f, 0.0f, 0.0f );

	this->setParent( parent );
	this->setGameEntities( gameEntities );
}

template <class T>
GEController<T>::GEController( const GEController& source)
{
	this->transformedValue = source.transformedValue;
	this->setParent( source.parent );	// okay to pass this pointer along
	this->setGameEntities( source.gameEntities );	// okay to pass this pointer along
}

template <class T>
GEController<T>::~GEController()
{
	// DO NOT DELETE parent and gameEntities... I know you want to because they are pointers... trust me on this one, just don't.
}

// Setters

template <class T>
void GEController<T>::setParent( const GEObject* parent )
{
	this->parent = parent;
}

template <class T>
void GEController<T>::setGameEntities( const std::map< std::string, GEObject* >* gameEntities )
{
	this->gameEntities = gameEntities;
}


// Getters

template <class T>
const GEObject* GEController<T>::getParent() const
{
	return this->parent;
}

template <class T>
const std::map< std::string, GEObject* >* GEController<T>::getGameEntities() const
{
	return this->gameEntities;
}


// Functions

template <class T>
GEController<T>* GEController<T>::clone() const
{
	return new GEController<T>(*this);
}

template <class T>
void GEController<T>::Control( T initialValue, double gameTime, double deltaTime)
{
	this->transformedValue = ( *new T() );  // completely ignore incoming data.
	
}

template <class T>
T GEController<T>::CalcTransform( T sourceValue )
{
	return sourceValue;  //return the source as the transformed.
}

typedef GEController<float> GEControllerf1;
typedef GEController<glm::vec2> GEControllerv2;
typedef GEController<glm::vec3> GEControllerv3;


#endif /* GECONTROLLER_H */