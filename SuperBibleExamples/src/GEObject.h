#ifndef GEOBJECT_H
#define GEOBJECT_H

/**
	GEObject (Graphics Engine Object)
	Purpose:  Base entity class.  Every object in the game should have this class as its root.
	Author: Jeff Adams

*/

#include <string>
#include <glm\glm.hpp>

#include "GEController.h"



class GEObject
{
private:
	// Members
	std::string id;		// unique id for this object.  Is this necessary?
	std::string name;	// non-unique name;

	// Transforms
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	// Transform Controllers
	GEController* positionController;
	GEController* rotationController;
	GEController* scaleController;

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
	
	virtual void setPositionController( GEController* positionController );
	virtual void setRotationController( GEController* rotationController );	
	virtual void setScaleController( GEController* scaleController);

	void setVisible( const bool visible );
	void setMesh( const std::string mesh );
	void setMaterial( const std::string material );


	// Getters
	std::string getID() const;
	std::string getName() const;
	
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	
	const GEController* getPositionController() const;
	const GEController* getRotationController() const;
	const GEController* getScaleController() const;

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

};


#endif /* GEOBJECT_H */