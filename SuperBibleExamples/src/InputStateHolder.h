#ifndef INPUTSTATEHOLDER_H
#define INPUTSTATEHOLDER_H

/**
	InputStateHolder
	Holds a pointer to the input state object.  It was easier to pass to other 
	objects if GEInputState was not a child of GEObject ( circular includes were causing problems compiling ) but still 
	needed a way to pass it to the graphics engine in the gameEntities list

	Author: Jeff Adams
*/

#include "GEObject.h"
#include "GEInputState.h"

class InputStateHolder: public GEObject
{
private:
	// Members

	const GEInputState* inputState;		// Pointer to the input state

public:

	// Structors
	InputStateHolder()
		:GEObject() {
		this->setInputState( nullptr );
		this->setVisible( false); };
	InputStateHolder( const GEInputState* inputState )
		:GEObject() { 
		this->setInputState( inputState ); 
		this->setVisible( false ); };
	InputStateHolder( const InputStateHolder& source)
		:GEObject( source ) {
		this->setInputState( source.inputState ); 
		this->setVisible( source.visible ); };
	// destructor not needed... pointer owned by something else.

	// Setters

	void setInputState( const GEInputState* inputState ) { this->inputState = inputState; };

	// Getters

	const GEInputState* getInputState() const { return this->inputState; };

	// Functions
	virtual std::string getClassName() const {return "InputStateHolder"; };
	virtual GEObject* clone() const{ return new InputStateHolder(*this); };
};

#endif /* INPUTSTATEHOLDER_H */