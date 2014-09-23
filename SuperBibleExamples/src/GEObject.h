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

#include "GEController.h"
#include "GEProperty.h"

template <class T>
class GEController;

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

	// Transform Controllers
	//std::vector<GEController*> positionControllers;
	//std::vector<GEController*> rotationControllers;
	//std::vector<GEController*> scaleControllers;

	// Display
	bool visible;				// draw it?
	std::string mesh;			// path to the mesh.
	std::string material;		// path to the material
	
public:
	// Structors
	GEObject();
	GEObject( glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, std::string name = std::string( "" ) );
	virtual ~GEObject();

	// Setters
	void setName( const std::string name );
	
	virtual void setPosition(const glm::vec3 position);
	virtual void setRotation(const glm::vec3 rotation);  // in Radians!
	virtual void setScale(const glm::vec3 scale);

	void setVisible( const bool visible );
	void setMesh( const std::string mesh );
	void setMaterial( const std::string material );


	// Getters
	std::string getID() const;
	std::string getName() const;
	
	glm::vec3 getBasePosition() const;
	glm::vec3 getBaseRotation() const;
	glm::vec3 getBaseScale() const;
	
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
		getTransformedPosition()
		Returns the combined result of the position controllers.
	*/
	virtual glm::vec3 getTransformedPosition() const;

	/**
		getTransformedRotation()
		Returns the combined result of the rotation controllers.
	*/
	virtual glm::vec3 getTransformedRotation() const;

	/**
		getTransformedScale()
		Returns the combined result of the scale controllers.
	*/
	virtual glm::vec3 getTransformedScale() const;

	virtual void addPositionController( GEControllerv3* positionController );
	virtual void addRotationController( GEControllerv3* rotationController );	
	virtual void addScaleController( GEControllerv3* scaleController);

	virtual void removePositionController( const unsigned int index );
	virtual void removeRotationController( const unsigned int index );	
	virtual void removeScaleController( const unsigned int index);

	/**
		setControllerGameEntitiesPointer()
		Sets the pointer to the gameEntities so the controller can access the properties of other game entities.
		@ param gameEntities - pointer to the gameEntities
		@ return void
	*/
	virtual void setControllerGameEntitiesPointer( const std::map< std::string, GEObject* >* gameEntities);

};


#endif /* GEOBJECT_H */