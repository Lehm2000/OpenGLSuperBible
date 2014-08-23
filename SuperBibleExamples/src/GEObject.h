#ifndef GEOBJECT_H
#define GEOBJECT_H

/**
	GEObject (Graphics Engine Object)
	Purpose:  Base entity class.  Every object in the game should have this class as its root.
	Author: Jeff Adams

*/

#include <string>

#include "vmath.h"


class GEObject
{
private:
	// Members
	std::string id;		// unique id for this object.
	std::string name;	// non-unique name;

	// spacial - stuff TODO replace vmath library
	vmath::vec3 position;
	vmath::vec3 rotation;
	vmath::vec3 scale;

	// Spacial Velocities.
	vmath::vec3 positionVel;
	vmath::vec3 rotationVel;
	vmath::vec3 scaleVel;

	// Display
	bool visible;				// draw it?
	std::string mesh;			// path to the mesh.
	std::string material;		// path to the material
	
public:
	// Structors
	GEObject();
	GEObject( vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, std::string name = std::string( "" ) );
	virtual ~GEObject();

	// Setters
	void setName( const std::string name );
	
	void setPosition(const vmath::vec3 position);
	void setRotation(const vmath::vec3 rotation);
	void setScale(const vmath::vec3 scale);
	
	void setPositionVel(const vmath::vec3 positionVel);
	void setRotationVel(const vmath::vec3 rotationVel);
	void setScaleVel(const vmath::vec3 scaleVel);

	void setVisible( const bool visible );
	void setMesh( const std::string mesh );
	void setMaterial( const std::string material );


	// Getters
	std::string getID() const;
	std::string getName() const;
	
	vmath::vec3 getPosition() const;
	vmath::vec3 getRotation() const;
	vmath::vec3 getScale() const;
	
	vmath::vec3 getPositionVel() const;
	vmath::vec3 getRotationVel() const;
	vmath::vec3 getScaleVel() const;

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
	vmath::mat4 GetTransformMatrix();

};


#endif /* GEOBJECT_H */