

#include "GraphicsEngineOpenGL.h"
#include "GameEngine.h"
#include "CameraObject.h"
#include "CameraPerspective.h"
#include "GEControllerOscillator.h"
//#include "GEControllerLookAt.h"
#include "GEControllerInputMousePositionX.h"
#include "GEControllerInputMousePositionY.h"
#include "GEControllerInputMouseScrollY.h"
#include "InfoGameVars.h"
#include "InfoGameEngineSettings.h"
#include "GEConstants.h"
#include "GEInputState.h"
#include "TypeDefinitions.h"
#include "MUMesh.h"
#include "MeshUtilities.h"

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

	if (graphics != nullptr)
	{
		delete graphics;	// instantiated with new
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

bool GameEngine::Initialize()
{
	// Engine Setup Here.

	bool success = true;

	// Setup the engine settings objects

	// Add the game engine settings object
	InfoGameEngineSettings* gameEngineSettings = new InfoGameEngineSettings();
	gameEngineSettings->setRenderMode( GE_RENDERMODE_FULL );
	gameEngineSettings->setShowBoundingBoxes( true );
	gameEngineSettings->setViewportWidth( 1280 );
	gameEngineSettings->setViewportHeight( 720 );
	AddEntity( "SYS_GameEngine_Settings", gameEngineSettings );  //add the options to the entity list.

	// Add the game variable object
	GEObject* gameVars = new InfoGameVars();
	AddEntity( "SYS_Game_Vars", gameVars );

	// Add the input state object... keeps track of current inputs.
	GEObject* inputState = new GEInputState( GEvec2( ((InfoGameEngineSettings*)gameEngineSettings)->getViewportWidth()/2, ((InfoGameEngineSettings*)gameEngineSettings)->getViewportHeight()/2) );
	AddEntity( "SYS_Input_State", inputState );

	// create the graphics engine
	graphics = new GraphicsEngineOpenGL( &gameEntities );	// Create the graphics engine object.  TODO allow more than one type of GE to be used.

	// load some default materials TODO: Move somewhere else
	LoadMaterial("tessellation_test");
	LoadMaterial("tessellation_testBezier");
	LoadMaterial("geometry_testNormals");
	LoadMaterial("geometry_testNormalsRay");
	LoadMaterial("default");
	LoadMaterial("default_wTexture");
	LoadMaterial( "boundingBox" );

	// Buffer the default meshes... TODO: Move somewhere else
	LoadMesh( "beziersphere" );
	LoadMesh( "sphere" );
	LoadMesh( "testBox" );
	

	return success;
}

void GameEngine::Update()
{
	// Do input--------------------------------------------------------------------------------------------

	// Get a reference to the input state

	GEInputState* inputState = (GEInputState*)gameEntities.GetObject( "SYS_Input_State" );

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
	InfoGameVars* gameVars = (InfoGameVars*)gameEntities.GetObject( "SYS_Game_Vars" );
	
	//update the game time
	gameVars->setCurrentFrameTime( getGameTime() );
	
	//double timeDelta = getGameTime() - lastFrameTime;

	// Update entities--------------------------------------------------------------------------------------
	gameEntities.UpdateObjects( getGameTime(), gameVars->getDeltaFrameTime() );

	// Update the last frametime
	this->lastFrameTime = getGameTime();
}

void GameEngine::Render()
{
	if ( graphics != nullptr )
		//graphics->TutRender( getGameTime() );  // tutorial/test renderer
		graphics->Render( getGameTime() ); // game renderer
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
		if ( !gameEntities.ContainsObject( entityName ) )
		{
			// if it is not, add it.

			// pass the gameEntities pointer to the entity here... which it will pass to the controllers.
			entity->setControllerGameEntitiesPointer( &gameEntities );

			success = gameEntities.AddObject( entityName, entity );

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

		}
	}

	return success;
}

void GameEngine::RemoveEntity( const std::string entityName)
{
	gameEntities.RemoveObject( entityName );
}

GEObject* GameEngine::GetEntity( const std::string entityName )
{
	return gameEntities.GetObject( entityName );
}

bool GameEngine::LoadMesh( std::string meshPath )
{
	MeshUtilities meshUtil;

	// Check if mesh already loaded

	if (!graphics->isMeshBuffered( meshPath ))
	{
		// Load mesh here... for now we'll hard code a mesh.

		if( meshPath == "default" || meshPath =="cube")
		{
			GEVertex meshVerts[8];

			//define a cube
			meshUtil.FillGEVertex( &meshVerts[0], -0.25f, -0.25f, -0.25f,	0.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[1], -0.25f, 0.25f, -0.25f,		0.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[2], 0.25f, -0.25f, -0.25f,		1.0f, 0.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[3], 0.25f, 0.25f, -0.25f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[4], 0.25f, -0.25f, 0.25f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[5], 0.25f, 0.25f, 0.25f,		1.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[6], -0.25f, -0.25f, 0.25f,		0.0f, 0.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[7], -0.25f, 0.25f, 0.25f,		0.0f, 1.0f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );

			// define the indexes

			unsigned int vertexIndecies[] =
			{
				0,1,2,3,4,5,6,7,0,1,0xFFFF,
				0,2,6,4,0xFFFF,
				1,3,7,5,0xFFFF
			};
			
			//pass to the engine here.

			MUMesh newMesh( GE_MESH_TRIANGLE_STRIP, 8, meshVerts, 21, vertexIndecies );
	
			graphics->BufferMesh( meshPath, &newMesh );
		}
		else if ( meshPath == "diamond" )
		{
			GEVertex meshVerts[6];

			//define a cube
			meshUtil.FillGEVertex( &meshVerts[0], 0.0f, 1.0f, 0.0f,		0.5f, 1.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[1], 0.0f, 0.0f, -1.0f,		0.5f, 0.5f, 0.0f, 1.0f,		0, 0, 0,	-0.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[2], 1.0f, 0.0f, 0.0f,		1.0f, 0.5f, 0.5f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[3], 0.0f, 0.0f, 1.0f,		0.5f, 0.5f, 1.0f, 1.0f,		0, 0, 0,	1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[4], -1.0f, 0.0f, 0.0f,		0.0f, 0.5f, 0.0f, 1.0f,		0, 0, 0,	1.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[5], 0.0f, -1.0f, 0.0f,		0.5f, 0.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

			// define the indexes

			unsigned int vertexIndecies[] =
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

			MUMesh newMesh( GE_MESH_TRIANGLE_STRIP, 6, meshVerts, 31, vertexIndecies );

			graphics->BufferMesh( meshPath, &newMesh );
		}

		else if ( meshPath == "sphere" )
		{
			const unsigned int numVSegments = 32;					// Vertical Segments
			const unsigned int numHSegments = numVSegments * 2;		// Horizontal Segments

			const unsigned int numVerts = ( (numVSegments - 1) * numHSegments ) + 2;

			const float radius = 0.5;

			GEVertex meshVerts[ numVerts ];

			// generate a sphere

			meshUtil.FillGEVertex( &meshVerts[0], 0.0f, radius, 0.0f,		0.5f, 1.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

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
					
					meshUtil.FillGEVertex( &meshVerts[ ( i * numHSegments ) + j - ( numHSegments - 1 )], finalx, y, z,		(finalx+radius)/(2.0f*radius),(y+radius)/(2.0f*radius), (z+radius)/(2.0*radius), 1.0f,		0.0f, 0.0f, 0.0f,	-0.0f, 1.0f );
				}
			}

			meshUtil.FillGEVertex( &meshVerts[ numVerts -1 ], 0.0f, radius * -1.0f, 0.0f,		0.5f, 0.0f, 0.5f, 1.0f,		0, 0, 0,	-0.0f, 1.0f );

			// now deal with the indexes... We'll create strips that start from the top of the sphere and go down.  
			// So we'll have the same number of strips as we do Horizontal segments

			// calculate the number needed.  

			const unsigned int numIndiciesStrip = ( 2 * numVSegments ) + 1;				// 2 indicies for each vertical level + 1 for the restart index
			const unsigned int numTotalIndicies = ( numIndiciesStrip ) * numHSegments;  //  * the number of H segments 


			unsigned int vertexIndicies[ numTotalIndicies ];

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
		
			MUMesh newMesh( GE_MESH_TRIANGLE_STRIP, numVerts, meshVerts, numTotalIndicies, vertexIndicies );

			graphics->BufferMesh( meshPath, &newMesh);

		}
		else if ( meshPath == "beziersphere" )
		{
			GEVertex meshVerts[8];

			//define a cube
			meshUtil.FillGEVertex( &meshVerts[0], -1.00f, -1.00f, -1.00f,	0.0f, 0.0f, 0.0f, 1.0f,		-1.00f, -1.00f, -1.00f,		-0.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[1], -1.00f, 1.00f, -1.00f,		0.0f, 1.0f, 0.0f, 1.0f,		-1.00f, 1.00f, -1.00f,		-0.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[2], 1.00f, -1.00f, -1.00f,		1.0f, 0.0f, 0.0f, 1.0f,		1.00f, -1.00f, -1.00f,		1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[3], 1.00f, 1.00f, -1.00f,		1.0f, 1.0f, 0.0f, 1.0f,		1.00f, 1.00f, -1.00f,		1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[4], 1.00f, -1.00f, 1.00f,		1.0f, 0.0f, 1.0f, 1.0f,		1.00f, -1.00f, 1.00f,		1.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[5], 1.00f, 1.00f, 1.00f,		1.0f, 1.0f, 1.0f, 1.0f,		1.00f, 1.00f, 1.00f,		-0.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[6], -1.00f, -1.00f, 1.00f,		0.0f, 0.0f, 1.0f, 1.0f,		-1.00f, -1.00f, 1.00f,		1.0f, -0.0f );
			meshUtil.FillGEVertex( &meshVerts[7], -1.00f, 1.00f, 1.00f,		0.0f, 1.0f, 1.0f, 1.0f,		-1.00f, 1.00f, 1.00f,		1.0f, -0.0f );

			// define the indexes

			unsigned int vertexIndicies[] =
			{
				0,1,2,3,0xFFFF,
				2,3,4,5,0xFFFF,
				4,5,6,7,0xFFFF,
				6,7,0,1,0xFFFF,
				0,2,6,4,0xFFFF,
				1,3,7,5,0xFFFF
			};

			MUMesh newMesh( GE_MESH_TRIANGLE_STRIP, 8, meshVerts, 30, vertexIndicies );

			graphics->BufferMesh( meshPath, &newMesh );
		}
		else if ( meshPath == "plane" )
		{
			GEVertex meshVerts[4];

			meshUtil.FillGEVertex( &meshVerts[0], -1.00f, 1.00f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.00f, 0.00f, -1.00f,		0.0f, 0.0f );
			meshUtil.FillGEVertex( &meshVerts[1], 1.00f, 1.00f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.00f, 0.00f, -1.00f,		1.0f, 0.0f );
			meshUtil.FillGEVertex( &meshVerts[2], -1.00f, -1.00f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.00f, 0.00f, -1.00f,		0.0f, 1.0f );
			meshUtil.FillGEVertex( &meshVerts[3], 1.00f, -1.00f, 0.0f,		1.0f, 1.0f, 1.0f, 1.0f,		0.00f, 0.00f, -1.00f,		1.0f, 1.0f );

			unsigned int vertexIndicies[] =
			{
				0,1,2,3
			};

			MUMesh newMesh( GE_MESH_TRIANGLE_STRIP, 4, meshVerts, 4, vertexIndicies );

			graphics->BufferMesh( meshPath, &newMesh );
		}

		else
		{
			// its not one of our standard meshes.  let's try to load it.

			std::string fullMeshPath = "./meshes/" + meshPath + ".ASE";

			MUMesh loadedMesh = meshUtil.LoadASE( fullMeshPath );

			graphics->BufferMesh( meshPath, &loadedMesh );
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
