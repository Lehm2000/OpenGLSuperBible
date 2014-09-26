#ifndef GEOBJECT_H
#define GEOBJECT_H

/**
	GEObject (Graphics Engine Object)
	Purpose:  Base entity class.  Every object in the game should have this class as its root.
	Author: Jeff Adams

*/

#include <string>
#include <vector>
#include <glm\glm.hpp>

#include "TypeDefinitions.h"
#include "GEController.h"
#include "GEProperty.h"
#include "GEObjectContainer.h"


template <class T>
class GEController;

class GEObjectContainer;

class GEObject
{
protected:
	// Members
	std::string id;		// unique id for this object.  TODO Is this necessary?
	std::string name;	// non-unique name;

	// Transforms
	GEPropertyv3 position;
	GEPropertyv3 rotation;
	GEPropertyv3 scale;

	// Display
	bool visible;				// draw it?
	std::string mesh;			// path to the mesh.
	std::string material;		// path to the material
	
public:
	// Structors
	GEObject();
	GEObject( const GEObject& source );
	GEObject( GEvec3 position, GEvec3 rotation, GEvec3 scale, std::string name = std::string( "" ) );
	virtual ~GEObject();

	// Setters
	void setName( const std::string name );
	
	//virtual void setPosition(const GEvec3 position);
	//virtual void setRotation(const GEvec3 rotation);  // in Radians!
	//virtual void setScale(const GEvec3 scale);

	void setVisible( const bool visible );
	void setMesh( const std::string mesh );
	void setMaterial( const std::string material );


	// Getters
	std::string getID() const;
	std::string getName() const;
	
	GEPropertyv3* getPosition();
	GEPropertyv3* getRotation();
	GEPropertyv3* getScale();
	
	/*  // these might be irrelevant now
	const GEController* getPositionController() const;
	const GEController* getRotationController() const;
	const GEController* getScaleController() const;
	*/

	bool isVisible() const;
	std::string getMesh() const;
	std::string getMaterial() const;

	// Comparison
	bool operator==( const GEObject& other ) const;
	bool operator!=( const GEObject& other ) const;
	bool operator<( const GEObject& other ) const;
	bool operator>( const GEObject& other ) const;
	bool operator<=( const GEObject& other ) const;
	bool operator>=( const GEObject& other ) const;

	// Functions
	void GenerateID();
	virtual std::string getClassName();

	glm::mat4 GetTransformMatrix();

	/**
		Update()
		@param gameTime - time (in seconds) passed since game began.
		@param deltaTime - time (in seconds) passed since last frame.
	*/
	virtual void Update( const double gameTime, const double deltaTime);

	/**
		clone()
		Creates a copy of the object and returns it.
	*/
	virtual GEObject* clone() const;

	/**
		setControllerGameEntitiesPointer()
		Sets the pointer to the gameEntities so the controller can access the properties of other game entities.
		@ param gameEntities - pointer to the gameEntities
		@ return void
	*/
	virtual void setControllerGameEntitiesPointer( const GEObjectContainer* gameEntities);

};


#endif /* GEOBJECT_H */