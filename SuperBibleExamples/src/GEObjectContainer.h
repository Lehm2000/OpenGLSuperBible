#ifndef GEOBJECTCONTAINER_H
#define GEOBJECTCONTAINER_H

/**
	ObjectContainer
	Purpose:  To hold encapsulate the objects for the game world so that it can be
	easily passed to the graphics engine.  This will also allow the container type
	to be replaced more easily if necessary.
	Author: Jeff Adams

*/

#include <map>

#include "GEObject.h"
#include "GEInputState.h"

class GEObject;
class GEInputState;

class GEObjectContainer
{
private:
	std::map< std::string, GEObject* > objects;

public:
	GEObjectContainer();
	GEObjectContainer( GEObjectContainer& source );
	~GEObjectContainer();

	/**
		AddEntity()
		Adds an object/entity to the gameEntities map
		@param entityName - key for map so that the object can be referenced later.
		@param entity - a pointer to the object itself.
		@return - if successful
	*/
	bool AddObject( const std::string entityName, GEObject* entity );

	/**
		RemoveEntity()
		Removes an object/entity from the gameEntities map
		@param entityName - key for the object in the map to be removed
	*/
	void RemoveObject( const std::string entityName );

	/**
		ContainsObject
		Does it contain a certain object
		@param entityName - key for object to look for
		@return - true if the object found.
	*/
	bool ContainsObject( const std::string entityName );

	/**
		GetEntity()
		Returns a pointer to object specified by entityName
		@param entityName - name of object to return
		@return - the pointer to the object, or nullptr if not found
	*/
	const GEObject* GetObject( const std::string entityName ) const;
	GEObject* GetObject( const std::string entityName );

	/**
		First()
		Returns a const iterator to the first object
	*/
	std::map< std::string, GEObject*>::const_iterator First() const;

	/**
		Last()
		REturns a const iterator to the last object
	*/
	std::map< std::string, GEObject*>::const_iterator Last() const;

	/**
		UpdateObjects()
		Calls the update function of all the objects
	*/
	void UpdateObjects( double gameTime, double deltaTime, const GEInputState* inputState );
};

#endif /* GEOBJECTCONTAINER_H */