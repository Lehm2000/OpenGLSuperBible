
#include <glm\glm.hpp>

#include "InputItem.h"

InputItem::InputItem()
{
	this->setInputType( GE_INPUT_NONE );
	this->setInputIndex( GE_KEY_UNKNOWN );
	this->setInputAction( GE_ACTION_NONE );
	this->setInputPosition( glm::vec2( 0.0f, 0.0f ) );
}

InputItem::InputItem( unsigned int inputType, unsigned int inputIndex, unsigned int inputAction, glm::vec2 inputPosition )
{
	this->setInputType( inputType );
	this->setInputIndex( inputIndex );
	this->setInputAction( inputAction );
	this->setInputPosition( inputPosition );
}

void InputItem::setInputType( const unsigned int inputType )
{
	this->inputType = inputType;
}

void InputItem::setInputIndex( const unsigned int inputIndex )
{
	this->inputIndex = inputIndex;
}

void InputItem::setInputAction( const unsigned int inputAction )
{
	this->inputAction = inputAction;
}

void InputItem::setInputPosition( const glm::vec2 inputPosition )
{
	this->inputPosition = inputPosition;
}


// Getters
unsigned int InputItem::getInputType() const
{
	return this->inputType;
}

unsigned int InputItem::getInputIndex() const
{
	return this->inputIndex;
}

unsigned int InputItem::getInputAction() const
{
	return this->inputAction;
}

glm::vec2 InputItem::getInputPosition() const
{
	return this->inputPosition;
}

