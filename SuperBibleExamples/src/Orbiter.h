#ifndef ORBITER_H
#define ORBITER_H

/**
	Orbiter
	GEObject that revolves around another object.  Other objects can then be 
	linked to this object to give them the ability to rotate.  Tried to
	implement this functionality as a controller but became too convoluted.

	Author: Jeff Adams
*/

#include <string>

#include "GEObject.h"


class Orbiter: public GEObject
{
private:
	// Members

	std::string orbitTargetName;		// name of object to orbit... TODO investigate using a pointer instead.  Definitely would be faster... but might not be reliable, how would you know if the object was deleted?
	GEPropertyv3 orbitAngle;			// orbit angle
	GEPropertyf1 orbitDistance;			// how far are we from the target.

public:
	// Structors

	Orbiter();
	Orbiter( GEvec3 position, GEvec3 rotation, GEvec3 scale, std::string orbitTargetName, GEvec3 orbitAngle, float orbitDistance );
	Orbiter( const Orbiter& source );
	~Orbiter();

	// Setters

	void setOrbitTargetName( const std::string orbitTargetName );

	void setOrbitAngleStart( const GEvec3 start ) { this->orbitAngle.setValue( start ); };
	void setOrbitAngleMin( const GEvec3 min ) { this->orbitAngle.setMin( min ); };
	void setOrbitAngleUseMin( const bool useMin ) { this->orbitAngle.setUseMin( useMin ); };
	void setOrbitAngleMax( const GEvec3 max ) { this->orbitAngle.setMax( max ); };
	void setOrbitAngleUseMax( const bool useMax ) { this->orbitAngle.setUseMax( useMax ); };

	void setOrbitDistanceStart( const float start ) { this->orbitDistance.setValue( start ); };
	void setOrbitDistanceMin( const float min ) { this->orbitDistance.setMin( min ); };
	void setOrbitDistanceUseMin( const bool useMin ) { this->orbitDistance.setUseMin( useMin ); };
	void setOrbitDistanceMax( const float max ) { this->orbitDistance.setMax( max ); };
	void setOrbitDistanceUseMax( const bool useMax ) { this->orbitDistance.setUseMax( useMax ); };
	
	// Getters

	std::string getOrbitTargetName() const;

	virtual GEvec3 getPositionFinal() const { return this->GetOrbitPosition(); };

	GEvec3 getOrbitAngleStart() const { return this->orbitAngle.getBaseValue(); };
	GEvec3 getOrbitAngleFinal() const { return this->orbitAngle.getFinalValue(); };
	GEvec3 getOrbitAngleMin() const { return this->orbitAngle.getMinValue(); };
	bool getOrbitAngleUseMin() const { return this->orbitAngle.getUseMin(); };
	GEvec3 getOrbitAngleMax() const { return this->orbitAngle.getMaxValue(); };
	bool getOrbitAngleUseMax() const { return this->orbitAngle.getUseMax(); };

	float getOrbitDistanceStart() const { return this->orbitDistance.getBaseValue(); };
	float getOrbitDistanceFinal() const { return this->orbitDistance.getFinalValue(); };
	float getOrbitDistanceMin() const { return this->orbitDistance.getMinValue(); };
	bool getOrbitDistanceUseMin() const { return this->orbitDistance.getUseMin(); };
	float getOrbitDistanceMax() const { return this->orbitDistance.getMaxValue(); };
	bool getOrbitDistanceUseMax() const { return this->orbitDistance.getUseMax(); };
	
	// Functions

	virtual std::string getClassName() const;

	void addOrbitAngleController( GEControllerv3* controller, const GEObject* parent );
	void removeOrbitAngleController( const unsigned int index );
	void addOrbitDistanceController( GEControllerf1* controller, const GEObject* parent );
	void removeOrbitDistanceController( const unsigned int index );
	
	/**
	*/
	GEvec3 GetOrbitPosition() const;

	/**
		GetTransformMatrix()
		Must override this as it will behave completely differently from the base version.
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

#endif /* ORBITER_H */