


#include "GameEngine.h"
#include "CameraObject.h"
#include "CameraPerspective.h"
#include "InfoViewport.h"
#include "GEControllerOscillator.h"
#include "GEControllerLookAt.h"
#include "InfoGameVars.h"

// Structors
GameEngine::GameEngine()
{
	this->lastFrameTime = 0.0;
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



// Getters


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
	CameraObject* gameCam = nullptr;

	switch (camType)
	{
	case CAMTYPE_PERSPECTIVE:
		gameCam = new CameraPerspective( position, rotation, fov );
		break;
	case CAMTYPE_ORTHO:
		break;
	case CAMTYPE_2D:
		break;
	default:
		break;
	}

	if( gameCam != nullptr )
	{
		DestroyGameCam();  // Destroy an existing cam as we only support one camera presently.
		//gameCam->addPositionController( new GEControllerOscillator( glm::vec3( 0.0f, 1.0f, 0.0f ), 5.0f ) );
		gameCam->addRotationController( new GEControllerLookAt( "testObject") );
		AddEntity( "gameCam", gameCam );
	}
}

void GameEngine::DestroyGameCam()
{
	gameEntities.erase( "gameCam" );
}

bool GameEngine::Initialize()
{
	// Engine Setup Here.

	bool success = true;

	// Setup the viewport options... is this the best place for this?

	GEObject* viewportOptions = new InfoViewport( 1280, 720 );
	AddEntity( "SYS_Viewport_Options", viewportOptions );  //add the options to the entity list.

	// Add the game variable object
	GEObject* gameVars = new InfoGameVars();
	AddEntity( "SYS_Game_Vars", gameVars );

	graphics = new GraphicsEngine( &gameEntities );	// Create the graphics engine object.  TODO allow more than one type of GE to be used.

	return success;
}

void GameEngine::Update()
{
	// get a reference to the game vars.
	std::map< std::string, GEObject* >::iterator vpIt = gameEntities.find("SYS_Game_Vars");
	InfoGameVars* gameVars = static_cast<InfoGameVars*>(vpIt->second);
	
	//update the game time
	gameVars->setCurrentFrameTime( getGameTime() );
	
	//double timeDelta = getGameTime() - lastFrameTime;

	// Do input

	// Update entities
	for ( std::map< std::string, GEObject* >::const_iterator it = gameEntities.begin(); it != gameEntities.end(); it++ )
	{
		it->second->Update( getGameTime(), gameVars->getDeltaFrameTime() );
	}

	// Update the last frametime
	this->lastFrameTime = getGameTime();
}

void GameEngine::Render()
{
	if ( graphics != nullptr )
		//graphics->Render( getGameTime() );  // tutorial/test renderer
		graphics->Render( getGameTime(), &gameEntities ); // game renderer
	// TODO what happens when its nullptr
}

bool GameEngine::isRunning() const
{
	bool running = true;

	if ( graphics != nullptr)
		running = !graphics->CheckWindowClose();  // TODO: there might be times when the window is closed and we want to keep going... like graphics reinitialization.

	return running;
}

bool GameEngine::AddEntity( const std::string entityName, GEObject* entity)
{
	bool success = false;

	if ( !entityName.empty() )
	{
		// Confirm that the entityName is not taken.
		if (gameEntities.find( entityName ) == gameEntities.end() )
		{
			// if it is not, add it.

			// pass the gameEntities pointer to the entity here... which it will pass to the controllers.
			entity->setControllerGameEntitiesPointer( &gameEntities );

			gameEntities.insert( std::pair< std::string, GEObject* >( entityName, entity ) );

			// if entity has a mesh specified load it.
			if ( !entity->getMesh().empty() )
			{
				LoadMesh( entity->getMesh() );
			}

			// do the same with the material.
			if ( !entity->getMaterial().empty() )
			{
				LoadMaterial( entity->getMaterial() );
			}

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

bool GameEngine::LoadMaterial( std::string materialPath )
{
	bool success = false;

	// Check if material already loaded

	if (!graphics->isMaterialBuffered( materialPath ))
	{
		success = graphics->BufferMaterial( materialPath );
	}
	else
	{
		success = true; // if it is already loaded it previously succeeded so we'll call this a win too.
	}

	return success;
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