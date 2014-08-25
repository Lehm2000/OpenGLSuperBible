


#include "GameEngine.h"
#include "CameraObject.h"
#include "PerspectiveCamera.h"

// Structors
GameEngine::GameEngine()
{
	gameCam = nullptr;
	graphics = nullptr;
}

GameEngine::GameEngine(const GameEngine& source)
{

}

GameEngine::~GameEngine()
{
	// Any shutdown stuff goes here
	DestroyGameCam();

	if (graphics != nullptr)
	{
		delete graphics;
		graphics = nullptr;
	}
}

// Setters
void GameEngine::setViewWidth( const unsigned short viewWidth )
{
	if ( viewWidth > 0 )
		this->viewWidth = viewWidth;
	// TODO what if not?
}

void GameEngine::setViewHeight( const unsigned short viewHeight )
{
	if ( viewHeight > 0 )
		this->viewHeight = viewHeight;
	// TODO what if not?
}

void GameEngine::setViewPort( const unsigned short viewWidth, const unsigned short viewHeight )
{
	if ( viewWidth > 0 && viewHeight > 0 )
	{
		setViewWidth( viewWidth );
		setViewHeight( viewHeight );
	}
}

// Getters
unsigned short GameEngine::getViewWidth() const
{
	return this->viewWidth;
}

unsigned short GameEngine::getViewHeight() const
{
	return this->viewHeight;
}

double GameEngine::getGameTime() const
{
	double gameTime = -1.0;

	if (graphics != nullptr)
	{
		gameTime = graphics->getCurrentTime();
	}
	// TODO what happens when it is nullptr?

	return gameTime;
}


//Functions
void GameEngine::CreateGameCam( const char camType, vmath::vec3 position, vmath::vec3 rotation, vmath::vec3 scale, float fov, vmath::vec3 targetPosition )
{
	DestroyGameCam();  // Destroy an existing cam.

	switch (camType)
	{
	case CAMTYPE_PERSPECTIVE:
		gameCam = new PerspectiveCamera( position, rotation, scale, fov, false, targetPosition );
		break;
	case CAMTYPE_PERSPECTIVE_TARGETED:
		gameCam = new PerspectiveCamera( position, rotation, scale, fov, true, targetPosition );
		break;
	case CAMTYPE_ORTHO:
		break;
	case CAMTYPE_ORTHO_TARGETED:
		break;
	case CAMTYPE_2D:
		break;
	default:
		break;
	}
}

void GameEngine::DestroyGameCam()
{
	if (gameCam != nullptr)
	{
		delete gameCam;
		gameCam = nullptr;
	}
}

bool GameEngine::Initialize()
{
	// Engine Setup Here.

	bool success = true;

	graphics = new GraphicsEngine;	// Create the graphics engine object.  TODO allow more than one type of GE to be used.

	return success;
}

void GameEngine::Update()
{
	//update the game world here.
}

void GameEngine::Render()
{
	if ( graphics != nullptr )
		graphics->Render( getGameTime() );  // TODO: need to pass the world state to the rendered.
	// TODO what happens when its nullprt
}

bool GameEngine::isRunning() const
{
	bool running = true;

	if ( graphics != nullptr)
		running = !graphics->CheckWindowClose();  // TODO: there might be times when the window is closed and we want to keep going... like graphics reinitialization.

	return running;
}
