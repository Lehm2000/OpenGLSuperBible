

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
	this->inputList.empty();

}

GraphicsEngine::GraphicsEngine( const std::map< std::string, GEObject* >* gameEntities )
{
	

	this->gameEntities = gameEntities;

	this->inputList.empty();
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

