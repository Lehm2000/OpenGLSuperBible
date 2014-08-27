


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
void GameEngine::CreateGameCam( const char camType, glm::vec3 position, glm::vec3 rotation, float fov, glm::vec3 targetPosition )
{
	DestroyGameCam();  // Destroy an existing cam.

	switch (camType)
	{
	case CAMTYPE_PERSPECTIVE:
		gameCam = new PerspectiveCamera( position, rotation, glm::vec3(1.0f, 1.0f, 1.0f), fov, false, targetPosition );
		break;
	case CAMTYPE_PERSPECTIVE_TARGETED:
		gameCam = new PerspectiveCamera( position, rotation, glm::vec3(1.0f, 1.0f, 1.0f), fov, true, targetPosition );
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

	// Do input

	// Update entities
}

void GameEngine::Render()
{
	if ( graphics != nullptr )
		graphics->Render( getGameTime() );  // tutorial/test renderer
		//graphics->Render( getGameTime(), this ); // game renderer
	// TODO what happens when its nullprt
}

bool GameEngine::isRunning() const
{
	bool running = true;

	if ( graphics != nullptr)
		running = !graphics->CheckWindowClose();  // TODO: there might be times when the window is closed and we want to keep going... like graphics reinitialization.

	return running;
}

bool GameEngine::AddEntity( const std::string entityName, GEObject entity)
{
	bool success = false;

	if ( !entityName.empty() )
	{
		// Confirm that the entityName is not taken.
		if (gameEntities.find( entityName ) == gameEntities.end() )
		{
			// if it is not, add it.

			gameEntities.insert( std::pair< std::string, GEObject >( entityName, entity ) );

			// if entity has a mesh specified load it.
			if ( entity.getMesh().empty() )
			{
				LoadMesh( entity.getMesh() );
			}

			// do the same with the material.

			success = true;
		}
	}

	return success;
}

void GameEngine::RemoveEntity( const std::string entityName)
{
	gameEntities.erase ( entityName );
}

bool GameEngine::LoadMesh( std::string meshPath )
{
	// Check if mesh already loaded

	if (!graphics->isMeshBuffered( meshPath ))
	{
		// Load mesh here... for now we'll hard code a mesh.

		GEVertex meshVerts[36];

		//define a cube
		FillGEVertex( &meshVerts[0], -0.25f, 0.25f, -0.25f,		0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[1], -0.25f, -0.25f, -0.25f,	0.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[2], 0.25f, -0.25f, -0.25f,		1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[3], 0.25f, -0.25f, -0.25f,		1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[4], 0.25f, 0.25f, -0.25f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[5], -0.25f, 0.25f, -0.25f,		0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[6], 0.25f, -0.25f, -0.25f,		1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[7], 0.25f, -0.25f, 0.25f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[8], 0.25f, 0.25f, -0.25f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[9], 0.25f, -0.25f, 0.25f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[10], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[11], 0.25f, 0.25f, -0.25f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[12], 0.25f, -0.25f, 0.25f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[13], -0.25f, -0.25f, 0.25f,	0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[14], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[15], -0.25f, -0.25f, 0.25f,	0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[16], -0.25f, 0.25f, 0.25f,		0.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[17], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[18], -0.25f, -0.25f, 0.25f,	0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[19], -0.25f, -0.25f, -0.25f,	0.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[20], -0.25f, 0.25f, 0.25f,		0.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, 0.25f );
		FillGEVertex( &meshVerts[21], -0.25f, -0.25f, -0.25f,	0.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[22], -0.25f, 0.25f, -0.25f,	0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[23], -0.25f, 0.25f, 0.25f,		0.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[24], -0.25f, -0.25f, 0.25f,	0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[25], 0.25f, -0.25f, 0.25f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[26], 0.25f, -0.25f, -0.25f,	1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[27], 0.25f, -0.25f, -0.25f,	1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
		FillGEVertex( &meshVerts[28], -0.25f, -0.25f, -0.25f,	0.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[29], -0.25f, -0.25f, 0.25f,	0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
		FillGEVertex( &meshVerts[30], -0.25f, 0.25f, -0.25f,	0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[31], 0.25f, 0.25f, -0.25f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[32], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[33], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
		FillGEVertex( &meshVerts[34], -0.25f, 0.25f, 0.25f,		0.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
		FillGEVertex( &meshVerts[35], -0.25f, 0.25f, -0.25f,	0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

		//pass to the engine here.
		graphics->BufferMesh( meshPath, meshVerts, 36 );

	}

	return true;  // Always a winner... for now.
}

void GameEngine::FillGEVertex( GEVertex* dest, float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float u, float v )
{
	dest->x = x;
	dest->y = y;
	dest->z = z;
	dest->r = r;
	dest->g = g;
	dest->b = b;
	dest->a = a;
	dest->nx = nx;
	dest->ny = ny;
	dest->nz = nz;
	dest->u = u;
	dest->v = v;
}