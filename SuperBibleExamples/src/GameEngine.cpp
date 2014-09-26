


#include "GameEngine.h"
#include "CameraObject.h"
#include "CameraPerspective.h"
#include "InfoViewport.h"
#include "GEControllerOscillator.h"
//#include "GEControllerLookAt.h"
#include "GEControllerInputMousePositionX.h"
#include "GEControllerInputMousePositionY.h"
#include "GEControllerInputMouseScrollY.h"
#include "InfoGameVars.h"
#include "GEConstants.h"
#include "GEInputState.h"
#include "TypeDefinitions.h"

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
void GameEngine::CreateGameCam( const char camType, GEvec3 position, GEvec3 rotation, float fov, GEvec3 targetPosition )
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
		//gameCam->addPositionController( new GEControllerOscillator( GEvec3( 0.0f, 1.0f, 0.0f ), 5.0f ) );
		//gameCam->addRotationController( new GEControllerLookAtv3( "testObject") );
		gameCam->getRotation()->addController( new GEControllerInputMousePositionXv3( GEvec3( 0.0f, -0.0025f, 0.0f ) ), gameCam );
		gameCam->getRotation()->addController( new GEControllerInputMousePositionYv3( GEvec3( -0.0025f, 0.0f, 0.0f ) ), gameCam );
		gameCam->getRotation()->setMax( GEvec3( 0.5f, 0.5f, 0.05f ) );
		gameCam->getRotation()->setUseMax( true );
		gameCam->getRotation()->setMin( GEvec3( -0.5f, -0.5f, 0.05f ) );
		gameCam->getRotation()->setUseMin( true );
		((CameraPerspective*)gameCam)->getFOV()->addController( new GEControllerInputMouseScrollYf1( -0.10f ), gameCam );
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

	// Add the input state object... keeps track of current inputs.
	GEObject* inputState = new GEInputState( GEvec2( ((InfoViewport*)viewportOptions)->getViewportWidth()/2, ((InfoViewport*)viewportOptions)->getViewportHeight()/2) );
	AddEntity( "SYS_Input_State", inputState );

	// create the graphics engine
	graphics = new GraphicsEngine( &gameEntities );	// Create the graphics engine object.  TODO allow more than one type of GE to be used.

	// load some default materials TODO: Move somewhere else
	LoadMaterial("tessellation_test");
	LoadMaterial("tessellation_testBezier");
	LoadMaterial("geometry_testNormals");
	LoadMaterial("geometry_testNormalsRay");
	LoadMaterial("default");

	// Buffer the default meshes... TODO: Move somewhere else
	LoadMesh( "beziersphere" );
	

	return success;
}

void GameEngine::Update()
{
	// Do input--------------------------------------------------------------------------------------------

	// Get a reference to the input state
	std::map< std::string, GEObject* >::iterator isIt = gameEntities.find("SYS_Input_State");
	GEInputState* inputState = static_cast<GEInputState*>(isIt->second);

	// Get pointer to the input queue in the graphics engine.
	std::queue< InputItem >* inputList = graphics->getInputList();
	
	// do the input here.

	inputState->ResetMouseScrollOffset();  // mouse offset needs to be reset at the beginning.

	while (inputList->size() > 0 )
	{
		InputItem curInput= inputList->front();

		// need a pressed bool for keys and buttons... assume not pressed.
		bool pressed = false;

		unsigned int inputType = curInput.getInputType();

		inputState->ResetMouseScrollOffset();

		switch ( inputType )
		{
		case GE_INPUT_KEY:

			// unless it tells us it was pressed
			if ( curInput.getInputAction() == GE_ACTION_PRESS || curInput.getInputAction() == GE_ACTION_REPEAT)
				pressed = true;

			// update the key state in the game engine.
			inputState->setKeyboardKey( curInput.getInputIndex(), pressed );

			break;
		case GE_INPUT_MOUSEBUTTON:
			if ( curInput.getInputAction() == GE_ACTION_PRESS )
				pressed = true;
			inputState->setMouseButton( curInput.getInputIndex(), pressed );

			break;
		case GE_INPUT_MOUSEPOSITION:
			inputState->setMousePosition( curInput.getInputPosition() );
			break;
		case GE_INPUT_MOUSESCROLL:
			inputState->setMouseScrollOffset( curInput.getInputPosition() );

			break;
		//default:
		}

		// remove the top item from the queue
		inputList->pop();
	}

	// Update game variables------------------------------------------------------------------------------
	
	// get a reference to the game vars.
	std::map< std::string, GEObject* >::iterator vpIt = gameEntities.find("SYS_Game_Vars");
	InfoGameVars* gameVars = static_cast<InfoGameVars*>(vpIt->second);
	
	//update the game time
	gameVars->setCurrentFrameTime( getGameTime() );
	
	//double timeDelta = getGameTime() - lastFrameTime;

	// Update entities--------------------------------------------------------------------------------------
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
		graphics->Render( getGameTime() );  // tutorial/test renderer
		//graphics->Render( getGameTime(), &gameEntities ); // game renderer
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

		if( meshPath == "default" || meshPath =="cube")
		{
			GEVertex meshVerts[8];

			//define a cube
			FillGEVertex( &meshVerts[0], -0.25f, -0.25f, -0.25f,	0.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
			FillGEVertex( &meshVerts[1], -0.25f, 0.25f, -0.25f,		0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
			FillGEVertex( &meshVerts[2], 0.25f, -0.25f, -0.25f,		1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			FillGEVertex( &meshVerts[3], 0.25f, 0.25f, -0.25f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			FillGEVertex( &meshVerts[4], 0.25f, -0.25f, 0.25f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
			FillGEVertex( &meshVerts[5], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
			FillGEVertex( &meshVerts[6], -0.25f, -0.25f, 0.25f,		0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			FillGEVertex( &meshVerts[7], -0.25f, 0.25f, 0.25f,		0.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );

			// define the indexes

			GLushort vertexIndecies[] =
			{
				0,1,2,3,4,5,6,7,0,1,0xFFFF,
				0,2,6,4,0xFFFF,
				1,3,7,5,0xFFFF
			};
			
			//pass to the engine here.
			graphics->BufferMesh( meshPath, meshVerts, 8, vertexIndecies, 21 );
		}
		else if ( meshPath == "diamond" )
		{
			GEVertex meshVerts[6];

			//define a cube
			FillGEVertex( &meshVerts[0], 0.0f, 1.0f, 0.0f,		0.5f, 1.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
			FillGEVertex( &meshVerts[1], 0.0f, 0.0f, -1.0f,		0.5f, 0.5f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
			FillGEVertex( &meshVerts[2], 1.0f, 0.0f, 0.0f,		1.0f, 0.5f, 0.5f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			FillGEVertex( &meshVerts[3], 0.0f, 0.0f, 1.0f,		0.5f, 0.5f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			FillGEVertex( &meshVerts[4], -1.0f, 0.0f, 0.0f,		0.0f, 0.5f, 0.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
			FillGEVertex( &meshVerts[5], 0.0f, -1.0f, 0.0f,		0.5f, 0.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

			// define the indexes

			GLushort vertexIndecies[] =
			{
				0,1,2,0xFFFF,
				0,2,3,0xFFFF,
				0,3,4,0xFFFF,
				0,4,1,0xFFFF,
				5,1,2,0xFFFF,
				5,2,3,0xFFFF,
				5,3,4,0xFFFF,
				5,4,1
				
				
			};
			
			//pass to the engine here.
			graphics->BufferMesh( meshPath, meshVerts, 6, vertexIndecies, 31 );
		}

		else if ( meshPath == "sphere" )
		{
			const unsigned int numVSegments = 32;					// Vertical Segments
			const unsigned int numHSegments = numVSegments * 2;		// Horizontal Segments

			const unsigned int numVerts = ( (numVSegments - 1) * numHSegments ) + 2;

			const float radius = 0.5;

			GEVertex meshVerts[ numVerts ];

			// generate a sphere

			FillGEVertex( &meshVerts[0], 0.0f, radius, 0.0f,		0.5f, 1.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

			for( unsigned int i = 1; i < numVSegments; i++ )
			{
				// specify the start point of this vertical level
				float y = cos( GE_PI * (float)i / (float)numVSegments ) * radius;
				float x = sin( GE_PI * (float)i / (float)numVSegments ) * radius;  // this is also the distance from the y-axis.
				//float z = 0.0f;

				// next revolve it around the y-axis
				for( unsigned int j = 0; j< numHSegments; j++ )
				{
					float finalx = cos( GE_PI * ( 2.0f * (float)j / (float)numHSegments) ) * x;
					float z = sin( GE_PI * ( 2.0f * (float)j / (float)numHSegments) ) * x;
					
					FillGEVertex( &meshVerts[ ( i * numHSegments ) + j - ( numHSegments - 1 )], finalx, y, z,		(finalx+radius)/(2.0f*radius),(y+radius)/(2.0f*radius), (z+radius)/(2.0*radius), 1.0f,		0.0f, 0.0f, 0.0f,	-0.0f, 1.0f );
				}
			}

			FillGEVertex( &meshVerts[ numVerts -1 ], 0.0f, radius * -1.0f, 0.0f,		0.5f, 0.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

			// now deal with the indexes... We'll create strips that start from the top of the sphere and go down.  
			// So we'll have the same number of strips as we do Horizontal segments

			// calculate the number needed.  

			const unsigned int numIndiciesStrip = ( 2 * numVSegments ) + 1;				// 2 indicies for each vertical level + 1 for the restart index
			const unsigned int numTotalIndicies = ( numIndiciesStrip ) * numHSegments;  //  * the number of H segments 


			GLushort vertexIndicies[ numTotalIndicies ];

			// now fill in the indices

			for ( unsigned int i = 0; i < numHSegments; i++)
			{
				for ( unsigned int j = 0; j < numVSegments + 1; j++)  // we add one for the final bottom row.
				{
					if ( j == 0) // first row
					{
						vertexIndicies[ i * numIndiciesStrip] = 0;
					}
					else if ( j == numVSegments ) // last row
					{
						vertexIndicies[ ( i * numIndiciesStrip ) + ( ( 2 * j ) - 1 ) ] = numVerts - 1;
						vertexIndicies[ ( i * numIndiciesStrip ) + ( 2 * j ) ] = 0xFFFF;  // restart index
					}
					else // one of the middle rows
					{
						unsigned int indexVal = ((j - 1)*numHSegments) +1 +i;

						vertexIndicies[ ( i * numIndiciesStrip ) + ( ( 2 * j ) - 1 ) ] = indexVal;
						
						if (indexVal == j*numHSegments)
						{
							indexVal -= numHSegments;
							
						}
						
						vertexIndicies[ ( i * numIndiciesStrip ) + ( ( 2 * j ) - 0 ) ] = indexVal +1;
					
					}
				}
			}
		


			graphics->BufferMesh( meshPath, meshVerts, numVerts, vertexIndicies, numTotalIndicies);

		}
		else if ( meshPath == "beziersphere" )
		{
			GEVertex meshVerts[8];

			//define a cube
			FillGEVertex( &meshVerts[0], -1.00f, -1.00f, -1.00f,	0.0f, 0.0f, 0.0f, 1.0f,		-1.00f, -1.00f, -1.00f,		-0.0f, 1.0f );
			FillGEVertex( &meshVerts[1], -1.00f, 1.00f, -1.00f,		0.0f, 1.0f, 0.0f, 1.0f,		-1.00f, 1.00f, -1.00f,		-0.0f, -0.0f );
			FillGEVertex( &meshVerts[2], 1.00f, -1.00f, -1.00f,		1.0f, 0.0f, 0.0f, 1.0f,		1.00f, -1.00f, -1.00f,		1.0f, -0.0f );
			FillGEVertex( &meshVerts[3], 1.00f, 1.00f, -1.00f,		1.0f, 1.0f, 0.0f, 1.0f,		1.00f, 1.00f, -1.00f,		1.0f, -0.0f );
			FillGEVertex( &meshVerts[4], 1.00f, -1.00f, 1.00f,		1.0f, 0.0f, 1.0f, 1.0f,		1.00f, -1.00f, 1.00f,		1.0f, 1.0f );
			FillGEVertex( &meshVerts[5], 1.00f, 1.00f, 1.00f,		1.0f, 1.0f, 1.0f, 1.0f,		1.00f, 1.00f, 1.00f,		-0.0f, 1.0f );
			FillGEVertex( &meshVerts[6], -1.00f, -1.00f, 1.00f,		0.0f, 0.0f, 1.0f, 1.0f,		-1.00f, -1.00f, 1.00f,		1.0f, -0.0f );
			FillGEVertex( &meshVerts[7], -1.00f, 1.00f, 1.00f,		0.0f, 1.0f, 1.0f, 1.0f,		-1.00f, 1.00f, 1.00f,		1.0f, -0.0f );

			// define the indexes

			GLushort vertexIndecies[] =
			{
				0,1,2,3,0xFFFF,
				2,3,4,5,0xFFFF,
				4,5,6,7,0xFFFF,
				6,7,0,1,0xFFFF,
				0,2,6,4,0xFFFF,
				1,3,7,5,0xFFFF
			};

			graphics->BufferMesh( meshPath, meshVerts, 8, vertexIndecies, 30 );
		}

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
	GEvec3 newNormal = glm::normalize( GEvec3(nx, ny, nz) );
	dest->nx = newNormal.x;
	dest->ny = newNormal.y;
	dest->nz = newNormal.z;
	dest->u = u;
	dest->v = v;
}