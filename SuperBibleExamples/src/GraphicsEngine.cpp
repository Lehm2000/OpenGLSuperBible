

#include <cstdio>
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>

#include "GraphicsEngine.h"
#include "GameEngine.h"
#include "TextureManager.h"
#include "ImageUtilities\ImageUtilities.h"
#include "CameraPerspective.h"
#include "InfoViewport.h"
#include "InfoGameVars.h"
#include "GEInputState.h"



GraphicsEngine::GraphicsEngine()
{
	this->gameEntities = nullptr;
	
	while ( !this->inputList.empty() )
		this->inputList.pop();

}

GraphicsEngine::GraphicsEngine( const GEObjectContainer* gameEntities )
{
	

	this->gameEntities = gameEntities;

	while (!this->inputList.empty() )
		this->inputList.pop();
}

GraphicsEngine::~GraphicsEngine()
{
	// clean up all pointers here

	//voa

	//vb
}

// setters

// getters

std::queue< InputItem >* GraphicsEngine::getInputList()
{
	return &inputList;
}

// Functions

void GraphicsEngine::QueueInputItem( InputItem input )
{
	inputList.push( input );
}

