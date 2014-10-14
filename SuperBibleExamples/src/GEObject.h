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
#include "GEObjectContainer.h"
#include "TypeDefinitions.h"


template <class T>
class GEController;

class GEObjectContainer;


class GEObject
{
protected:
	// Members
	std::string id;		// unique id for this object.  TODO Is this necessary?

	// Transforms
	GEPropertyv3 position;
	GEPropertyv3 rotation;
	GEPropertyv3 scale;

	// Display
	bool visible;				// draw it?
	std::string mesh;			// path to the mesh.
	std::string material;		// path to the material

	// other
	const GEObjectContainer* gameEntities;  /* pointer to the master gameEntity 
		list.  In case the controller needs to know the properties of some 
		other object in the world.  This one scares me a bit... I know its 
		const... but is there a better way to get this info? */

	
public:
	// Structors
	GEObject();
	GEObject( const GEObject& source );
	GEObject( GEvec3 position, GEvec3 rotation, GEvec3 scale );
	virtual ~GEObject();

	// Setters

	void setVisible( const bool visible );
	void setMesh( const std::string mesh );
	void setMaterial( const std::string material );

	void setGameEntities( const GEObjectContainer* gameEntities );

	void setPositionStart( const GEvec3 positionStart ) { this->position.setValue( positionStart ); };
	void setPositionMin( const GEvec3 positionMin ) { this->position.setMin( positionMin ); };
	void setPositionUseMin( const bool useMin ) { this->position.setUseMin( useMin ); };
	void setPositionMax( const GEvec3 positionMax ) { this->position.setMax( positionMax ); };
	void setPositionUseMax( const bool useMax ) { this->position.setUseMax( useMax ); };

	void setRotationStart( const GEvec3 start ) { this->rotation.setValue( start ); };
	void setRotationMin( const GEvec3 min ) { this->rotation.setMin( min ); };
	void setRotationUseMin( const bool useMin ) { this->rotation.setUseMin( useMin ); };
	void setRotationMax( const GEvec3 max ) { this->rotation.setMax( max ); };
	void setRotationUseMax( const bool useMax ) { this->rotation.setUseMax( useMax ); };

	void setScaleStart( const GEvec3 start ) { this->scale.setValue( start ); };
	void setScaleMin( const GEvec3 min ) { this->scale.setMin( min ); };
	void setScaleUseMin( const bool useMin ) { this->scale.setUseMin( useMin ); };
	void setScaleMax( const GEvec3 max ) { this->scale.setMax( max ); };
	void setScaleUseMax( const bool useMax ) { this->scale.setUseMax( useMax ); };


	// Getters
	std::string getID() const;
	
	GEvec3 getPositionStart() const { return this->position.getBaseValue(); };
	virtual GEvec3 getPositionFinal() const { return this->position.getFinalValue(); };
	GEvec3 getPositionMin() const { return this->position.getMinValue(); };
	bool getPositionUseMin() const { return this->position.getUseMin(); };
	GEvec3 getPositionMax() const { return this->position.getMaxValue(); };
	bool getPositionUseMax() const { return this->position.getUseMax(); };

	GEvec3 getRotationStart() const { return this->rotation.getBaseValue(); };
	virtual GEvec3 getRotationFinal() const { return this->rotation.getFinalValue(); };
	GEvec3 getRotationMin() const { return this->rotation.getMinValue(); };
	bool getRotationUseMin() const { return this->rotation.getUseMin(); };
	GEvec3 getRotationMax() const { return this->rotation.getMaxValue(); };
	bool getRotationUseMax() const { return this->rotation.getUseMax(); };

	GEvec3 getScaleStart() const { return this->scale.getBaseValue(); };
	virtual GEvec3 getScaleFinal() const { return this->scale.getFinalValue(); };
	GEvec3 getScaleMin() const { return this->scale.getMinValue(); };
	bool getScaleUseMin() const { return this->scale.getUseMin(); };
	GEvec3 getScaleMax() const { return this->scale.getMaxValue(); };
	bool getScaleUseMax() const { return this->scale.getUseMax(); };


	/*
	GEPropertyv3* getPositionProp();
	const GEPropertyv3* getPositionProp() const;
	GEPropertyv3* getRotationProp();
	const GEPropertyv3* getRotationProp() const;
	GEPropertyv3* getScaleProp();
	const GEPropertyv3* getScaleProp() const;
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
	virtual std::string getClassName() const;

	void addPositionController( GEControllerv3* controller, const GEObject* parent );
	void removePositionController( const unsigned int index );
	void addRotationController( GEControllerv3* controller, const GEObject* parent );
	void removeRotationController( const unsigned int index );
	void addScaleController( GEControllerv3* controller, const GEObject* parent );
	void removeScaleController( const unsigned int index );

	/**
	*/
	virtual glm::mat4 GetTransformMatrix() const;

	/**
		Update()
		@param gameTime - time (in seconds) passed since game began.
		@param deltaTime - time (in seconds) passed since last frame.
	*/
	virtual void Update( const double gameTime, const double deltaTime);
	
	/**
		ProcessInput
		Function for processing input from the user.  Meant to be stored in the
		inputFunction list as a pointer.  Takes the input state and passes it
		to the Controllable Properties.
	*/
	virtual void ProcessInput( const GEInputState* inputState );
	
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