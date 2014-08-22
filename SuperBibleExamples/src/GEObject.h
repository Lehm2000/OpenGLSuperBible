#ifndef GEOBJECT_H
#define GEOBJECT_H

/**
	GEObject (Graphics Engine Object)
	Purpose:  Base entity class.  Every object in the game should have this class as its root.

*/

#include <string>

#include "vmath.h"


class GEObject
{
private:
	// Members
	std::string id;		//unique id for this object.
	std::string name;	//non-unique name;

	// TODO replace vmath library
	vmath::vec3 position;
	vmath::vec3 orientation;
	vmath::vec3 scale;
	
public:
	// Structors
	GEObject();
	GEObject( vmath::vec3 position, vmath::vec3 orientation, vmath::vec3 scale, std::string name = std::string( "" ) );

	// Setters
	void setName( const std::string name );
	void setPosition(const vmath::vec3 position);
	void setOrientation(const vmath::vec3 orientation);
	void setScale(const vmath::vec3 scale);

	// Getters
	std::string getID() const;
	std::string getName() const;
	vmath::vec3 getPosition() const;
	vmath::vec3 getOrientation() const;
	vmath::vec3 getScale() const;

	// Comparison
	bool operator==( const GEObject& other ) const;
	bool operator!=( const GEObject& other ) const;
	bool operator<( const GEObject& other ) const;
	bool operator>( const GEObject& other ) const;
	bool operator<=( const GEObject& other ) const;
	bool operator>=( const GEObject& other ) const;

	// Functions
	void GenerateID();

};


#endif /* GEOBJECT_H */