
#include <fstream>
#include <cstring>
#include <vector>

#include "MeshUtilities.h"
#include "MUMesh.h"
#include "TypeDefinitions.h"
#include "GEUtilities.h"

// Functions

void MeshUtilities::FillGEVertex( GEVertex* dest, float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float u, float v )
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

MUMesh MeshUtilities::LoadASE( std::string meshPath )
{
	/*  
		Function for loading an Autodesk ASCII Scene Export file.  The format is
		slightly odd that the delimiters are not constant.  This means that each
		section requires custom code to import it properly.  Also the vertex
		information is not stored as graphics engines typically need them.  The
		normal and uv data is stored seperately.  This means that we need to
		duplicate the vertices to get this custom data.  The file also has a 
		bunch of data we are not interested in.  Instead of parsing the whole
		file we'll just be searching for the data we need to load.
	*/

	MUMesh returnMesh;

	std::ifstream inFile;
	char* fileBuffer ;

	inFile.open( meshPath.c_str(), std::ifstream::in );

	if( inFile.is_open() )
	{
		// first let's get the file size
		
		std::streampos fsize;

		fsize = inFile.tellg();
		inFile.seekg( 0, std::ios::end );
		fsize = inFile.tellg() - fsize;
		
		// allocate a buffer big enough to hold the entire file ( +1 ) for an extra 0 char.
		fileBuffer = (char*)malloc( (int)fsize + 1 );

		// set the first character to 0 so it appears as an empty string
		fileBuffer[0] = 0;

		// seek to the beginning again.
		inFile.seekg( 0, std::ios::beg );

		// read the file

		char lineBuffer[ 512 ];  // hopefully 512 is enough for the line buffer.
		const char nL = '\n';

		// go through each line and add it to the the filebuffer
		while ( !inFile.eof() )
		{
			inFile.getline(lineBuffer,512);
			
			strcat( fileBuffer, lineBuffer );
			strcat( fileBuffer, "\n" );
			
		}

		// convert it to string
		std::string fileBuffer2( fileBuffer );

		// now process the file.
		std::string searchFor, content;
		int contentPos;
		int contentEnd;

		// get the number of verticies *MESH_NUMVERTEX
		searchFor = "*MESH_NUMVERTEX";
		contentPos = fileBuffer2.find( searchFor ) + searchFor.length();
		contentEnd = fileBuffer2.find( "\n", contentPos );

		content = fileBuffer2.substr( contentPos, contentEnd - contentPos );
		int numMeshVertex = StringToNumber<int>( content );
		
		// get the number of faces *MESH_NUMFACES
		searchFor = "*MESH_NUMFACES";
		contentPos = fileBuffer2.find( searchFor ) + searchFor.length();
		contentEnd = fileBuffer2.find( "\n", contentPos );

		content = fileBuffer2.substr( contentPos, contentEnd - contentPos );
		int numMeshFaces = StringToNumber<int>( content );

		// get the number of texture vertices *MESH_NUMTVERTEX
		searchFor = "*MESH_NUMTVERTEX";
		contentPos = fileBuffer2.find( searchFor ) + searchFor.length();
		contentEnd = fileBuffer2.find( "\n", contentPos );

		content = fileBuffer2.substr( contentPos, contentEnd - contentPos );
		int numTVertex = StringToNumber<int>( content );

		// get the number of tvfaces *MESH_NUMTVFACES
		// this is the list of what texture vertices go with what faces.  This number should be the same as numfaces
		searchFor = "*MESH_NUMTVFACES";
		contentPos = fileBuffer2.find( searchFor ) + searchFor.length();
		contentEnd = fileBuffer2.find( "\n", contentPos );

		content = fileBuffer2.substr( contentPos, contentEnd - contentPos );
		int numTVFaces = StringToNumber<int>( content );

		// get the *MESH_VERTEX(s)  these are tab delimited
		// example line "*MESH_VERTEX    0	-0.5000	-0.5000	-0.5000"

		// allocate some memory for them.
		GEvec3* vertexList = ( GEvec3* )malloc( sizeof( GEvec3 ) * numMeshVertex );
		
		// set the start place to look.
		contentPos = 0;

		// loop until all the verticies are filled in.
		// TODO: might want to verify the number of mesh vertices match the number we expect.
		for( unsigned int i = 0; i < numMeshVertex; i++ )
		{
			searchFor = "*MESH_VERTEX ";  // have to include the space for this one.
			contentPos = fileBuffer2.find( searchFor, contentPos );
			contentPos = fileBuffer2.find( "\t", contentPos ) + 1;					// find the first tab
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the second tab
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			vertexList[i].x = StringToNumber<float>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the next tab.
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			vertexList[i].y = StringToNumber<float>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\n", contentPos);						// find the end of line characters
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value.

			vertexList[i].z = StringToNumber<float>( content );
		}

		// get the *MESH_FACE(s).  These are colon, space and tab delimited... seriously?
		// example line: *MESH_FACE    0:    A:    0 B:    2 C:    3 AB:    1 BC:    1 CA:    0	 *MESH_SMOOTHING 2 	*MESH_MTLID 1
		// we really only care about the vertex indices ABC.  So we can simply search for the colons.
		
		// allocate space for the face information
		unsigned int* faceList = (unsigned int*)malloc( sizeof( unsigned int ) * numMeshFaces * 3 );

		// set the start place to look.
		contentPos = 0;

		// loop until all the faces are filled in.
		// TODO: might want to verify the number of mesh faces match the number we expect.
		for( unsigned int i = 0; i < numMeshFaces; i++ )
		{
			searchFor = "*MESH_FACE  ";  // have to include the space for this one.
			contentPos = fileBuffer2.find( searchFor, contentPos );
			contentPos = fileBuffer2.find( "A:", contentPos ) + 2;					// find A:
			contentEnd = fileBuffer2.find( "B:", contentPos);						// find B:
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			faceList[ ( i * 3 ) + 0 ] = StringToNumber<int>( content );

			contentPos = contentEnd + 2;											
			contentEnd = fileBuffer2.find( "C:", contentPos);						// find B:
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			faceList[ ( i * 3 ) + 1 ] = StringToNumber<int>( content );

			contentPos = contentEnd + 2;											
			contentEnd = fileBuffer2.find( "AB:", contentPos);						// find AB:
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			faceList[ ( i * 3 ) + 2 ] = StringToNumber<int>( content );
		}

		// get the *MESH_TVERTLIST(3).  These are tab delimited.
		// example line "*MESH_TVERT 0	1.0000	0.0000	0.0000"
		// There are three values.  We only care about the first two.  the third is depth and only useful if using 3d textures.

		// allocate some memory for them.
		GEvec2* tVertList = ( GEvec2* )malloc( sizeof( GEvec2 ) * numTVertex );
		
		// set the start place to look.
		contentPos = 0;

		// loop until all the verticies are filled in.
		// TODO: might want to verify the number of mesh vertices match the number we expect.
		for( unsigned int i = 0; i < numTVertex; i++ )
		{
			searchFor = "*MESH_TVERT ";  // have to include the space for this one.
			contentPos = fileBuffer2.find( searchFor, contentPos );
			contentPos = fileBuffer2.find( "\t", contentPos ) + 1;					// find the first tab
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the second tab
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			tVertList[i].x = StringToNumber<float>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the next tab.
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			tVertList[i].y = StringToNumber<float>( content );

		}

		// get the *MESH_TFACE(s)  these are tab delimited
		// example line "*MESH_TFACE 0	0	2	3"

		// allocate some memory for them.
		unsigned int* tFaceList = (unsigned int*)malloc( sizeof( unsigned int ) * numTVFaces * 3 );
		
		// set the start place to look.
		contentPos = 0;

		// loop until all the verticies are filled in.
		// TODO: might want to verify the number of mesh vertices match the number we expect.
		for( unsigned int i = 0; i < numTVFaces; i++ )
		{
			searchFor = "*MESH_TFACE ";  // have to include the space for this one.
			contentPos = fileBuffer2.find( searchFor, contentPos );
			contentPos = fileBuffer2.find( "\t", contentPos ) + 1;					// find the first tab
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the second tab
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			tFaceList[ ( i * 3 ) + 0 ] = StringToNumber<int>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the next tab.
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			tFaceList[ ( i * 3 ) + 1 ] = StringToNumber<int>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\n", contentPos);						// find the end of line characters
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value.

			tFaceList[ ( i * 3 ) + 2 ] = StringToNumber<int>( content );
		}

		// get the *MESH_VERTEXNORMAL(s)  these are tab delimited
		// example line "*MESH_VERTEXNORMAL 0	0.0000	0.0000	-1.0000"
		// there is one for each vertice of the face.

		// allocate some memory for them.
		GEvec3* vertexNormalList = (GEvec3*)malloc( sizeof( GEvec3 ) * numTVFaces * 3 );
		
		// set the start place to look.
		contentPos = 0;

		// loop until all the verticies are filled in.
		// TODO: might want to verify the number of mesh vertices match the number we expect.
		for( unsigned int i = 0; i < numTVFaces * 3; i++ )
		{
			searchFor = "*MESH_VERTEXNORMAL ";  // have to include the space for this one.
			contentPos = fileBuffer2.find( searchFor, contentPos );
			contentPos = fileBuffer2.find( "\t", contentPos ) + 1;					// find the first tab
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the second tab
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			vertexNormalList[ i ].x = StringToNumber<float>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\t", contentPos);						// find the next tab.
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value

			vertexNormalList[ i ].y = StringToNumber<float>( content );

			contentPos = contentEnd + 1;											// the end position +1 should be the start of the next value
			contentEnd = fileBuffer2.find( "\n", contentPos);						// find the end of line characters
			content = fileBuffer2.substr( contentPos, contentEnd - contentPos );	// what is between should be the value.

			vertexNormalList[ i ].z = StringToNumber<float>( content );
		}

		// all the data should now be loaded!!!
		inFile.close();

		// now process the data.

		/*
			OpenGL typically likes the uv and normal data to be included with the 
			vert so that means for every unique normal/uv/position combination you need a
			vertex.  The ASE data is not recorded this way so that a vertex can have
			different attributes. There are two approaches to handling this.  Either 
			keep them seperate and let the shaders handle putting the data back 
			together or create unique vertices.  We'll be going with the later and 
			creating unique vertices.  This means more vertices to render, but fewer
			storage buffers.  TODO: investigate the other way.

		*/

		// need a place to hold the final vertices and index list

		std::vector<GEVertex> finalVertices;
		unsigned int* indicies = (unsigned int*)malloc( sizeof(unsigned int) * numMeshFaces * 3 );

		// go through each mesh face...
		// we'll also be swaping the y and z coordinates.  ase is z-up our engine is y-up

		for( unsigned int i = 0; i < numMeshFaces; i++ )
		{
			// ... and then each vertice of that face
			for ( unsigned int j = 0; j < 3; j++ )
			{
				// Build a GEVertex for it.
				GEVertex newVert;

				// get position
				newVert.x = vertexList[ faceList[ ( i * 3 ) + j ] ].x;
				newVert.z = vertexList[ faceList[ ( i * 3 ) + j ] ].y;
				newVert.y = vertexList[ faceList[ ( i * 3 ) + j ] ].z;

				// we'll put white for the color for now
				newVert.r = 1.0f;
				newVert.g = 1.0f;
				newVert.b = 1.0f;
				newVert.a = 1.0f;

				// next normal
				newVert.nx = vertexNormalList[ ( i * 3 ) + j ].x;
				newVert.ny = vertexNormalList[ ( i * 3 ) + j ].y;
				newVert.nz = vertexNormalList[ ( i * 3 ) + j ].z;

				// finally uv
				newVert.u = tVertList[ tFaceList[ ( i * 3 ) + j ] ].x;
				newVert.v = tVertList[ tFaceList[ ( i * 3 ) + j ] ].y;

				// find out if an identical vertex is already in the list
				bool foundVert = false;
				unsigned int foundVertNum = -1;

				for ( unsigned int k = 0; !foundVert && k < finalVertices.size(); k ++ )
				{
					if ( newVert == finalVertices[k])
					{
						foundVert = true;
						foundVertNum = k;
					}
				}

				if ( foundVert )
				{
					// if we found a vert use that as the index for this face vertice...
					indicies[ ( i * 3 ) + j ] = foundVertNum;
				}
				else
				{
					// ... otherwise add a new vertex to the list and use that index
					finalVertices.push_back( newVert );
					indicies[ ( i * 3 ) + j ] = finalVertices.size() - 1;
				}

				

			}
		}

		returnMesh.setMeshType ( GE_MESH_TRIANGLE_LIST );
		returnMesh.setVertices( finalVertices.size(), &finalVertices[0]);
		returnMesh.setIndicies( numMeshFaces * 3, indicies );

		// add some color

		ColorizeMesh( &returnMesh );

	}

	return returnMesh;
}

void MeshUtilities::ColorizeMesh( MUMesh* mesh )
{
	// get the extends/bounds of the mesh.
	GEBoundingBox meshBounds = mesh->GetMeshBounds();

	// now we colorize based on where the vertex is in the range
	for( unsigned int i = 0; i < mesh->getNumVerts(); i++ )
	{
		//const GEVertex* curVert = mesh->getVerticies();

		GEvec3 vertColor( 
			(mesh->getVerticies()[i].x - meshBounds.min.x) / ( meshBounds.max.x - meshBounds.min.x ), 
			(mesh->getVerticies()[i].y - meshBounds.min.y) / ( meshBounds.max.y - meshBounds.min.y ), 
			(mesh->getVerticies()[i].z - meshBounds.min.z) / ( meshBounds.max.z - meshBounds.min.z )
			);

		mesh->SetVertexColor( i, vertColor );
	}
}

bool operator==( const GEVertex& vert1, const GEVertex& vert2)
{
	bool same = true;

	if ( vert1.x != vert2.x )
		same  = false;
	if ( same && vert1.y != vert2.y )
		same  = false;
	if ( same && vert1.z != vert2.z )
		same  = false;
	if ( same && vert1.r != vert2.r )
		same  = false;
	if ( same && vert1.g != vert2.g )
		same  = false;
	if ( same && vert1.b != vert2.b )
		same  = false;
	if ( same && vert1.a != vert2.a )
		same  = false;
	if ( same && vert1.nx != vert2.nx )
		same  = false;
	if ( same && vert1.ny != vert2.ny )
		same  = false;
	if ( same && vert1.nz != vert2.nz )
		same  = false;
	if ( same && vert1.u != vert2.u )
		same  = false;
	if ( same && vert1.v != vert2.v )
		same  = false;

	return same;
}