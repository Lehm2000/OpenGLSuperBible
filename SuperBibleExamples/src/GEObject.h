#ifndef GEOBJECT_H
#define GEOBJECT_H

/**
	GEObject (Graphics Engine Object)
	Purpose:  Base entity class.  Every object in the game should have this class as its root.
	Author: Jeff Adams

*/

#include <string>
#include <glm\glm.hpp>

//#include "vmath.h"



class GEObject
{
private:
	// Members
	std::string id;		// unique id for this object.  Is this necessary?
	std::string name;	// non-unique name;

	// spacial
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	// Spacial Velocities.
	glm::vec3 positionVel;
	glm::vec3 rotationVel;
	glm::vec3 scaleVel;

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
	
	void setPosition(const glm::vec3 position);
	void setRotation(const glm::vec3 rotation);  // in Radians!
	void setScale(const glm::vec3 scale);
	
	void setPositionVel(const glm::vec3 positionVel);
	void setRotationVel(const glm::vec3 rotationVel);	// in Radians!
	void setScaleVel(const glm::vec3 scaleVel);

	void setVisible( const bool visible );
	void setMesh( const std::string mesh );
	void setMaterial( const std::string material );


	// Getters
	std::string getID() const;
	std::string getName() const;
	
	glm::vec3 getPosition() const;
	glm::vec3 getRotation() const;
	glm::vec3 getScale() const;
	
	glm::vec3 getPositionVel() const;
	glm::vec3 getRotationVel() const;
	glm::vec3 getScaleVel() const;

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

};


#endif /* GEOBJECT_H */