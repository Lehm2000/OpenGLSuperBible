
#include <cstdlib>
#include <cstring>
#include <algorithm>

#include <glm\glm.hpp>

#include "MUMesh.h"
#include "TypeDefinitions.h"

// Structors

MUMesh::MUMesh()
{
	this->setMeshType( GE_MESH_NONE );
	this->setVertices( 0, nullptr );
	this->setIndicies( 0, nullptr );
}

MUMesh::MUMesh( const unsigned char meshType, const unsigned int numVerts, const GEVertex* vertices, const unsigned int numIndicies, const unsigned int* indicies )
{
	this->setMeshType( meshType );
	this->setVertices( numVerts, vertices );
	this->setIndicies( numIndicies, indicies );
}

MUMesh::MUMesh( const MUMesh& source )
{
	this->meshType = source.meshType;
	// copy the verticies
	
	this->numVerts = source.numVerts;

	this->vertices = (GEVertex*)malloc( sizeof(GEVertex) * source.numVerts );
	for (unsigned int i = 0; i < source.numVerts; i++ )
	{
		this->vertices[i] = source.vertices[i];
	}

	// copy the indicies
	
	this->numIndicies = source.numIndicies;
	this->indicies = (unsigned int*)malloc( sizeof( unsigned int) * source.numIndicies );
	for (unsigned int i = 0; i < source.numIndicies; i++ )
	{
		this->indicies[i] = source.indicies[i];
	}

	this->boundingBox = source.boundingBox;
}

MUMesh::~MUMesh()
{
	free( this->vertices );  // allocated with malloc
	this->vertices = nullptr;
	
	free( this->indicies );	// allocated with malloc
	this->indicies = nullptr;
	
}


// Operators
	
MUMesh& MUMesh::operator=( MUMesh source )
{
	// create copy
	//MUMesh tmp( source );

	std::swap( meshType, source.meshType );
	std::swap( numVerts, source.numVerts );
	std::swap( vertices, source.vertices );
	std::swap( numIndicies, source.numIndicies );
	std::swap( indicies, source.indicies );
	std::swap( boundingBox, source.boundingBox );

	return *this;
}


// Setters

void MUMesh::setMeshType( const unsigned char meshType )
{
	this->meshType = meshType;
}

void MUMesh::setVertices( const unsigned int numVerts, const GEVertex* vertices )
{
	this->numVerts = numVerts;

	free (this->vertices);	// free previous data;

	if( numVerts == 0 )
	{
		this->vertices = nullptr;
	}
	else
	{
		this->vertices = (GEVertex*)malloc( sizeof(GEVertex) * numVerts );
		for ( unsigned int i = 0; i < numVerts; i++ )
		{
			this->vertices[i] = vertices[i];
		}
	}

	// set the new bounding box
	this->setBoundingBox();
}

void MUMesh::setIndicies( const unsigned int numIndicies, const unsigned int* indicies )
{
	this->numIndicies = numIndicies;

	free( this->indicies );	// free previous data... if any there.

	if( numIndicies == 0 )
	{
		this->indicies = nullptr;
	}
	else
	{
		this->indicies = (unsigned int*)malloc( sizeof(unsigned int) * numIndicies );
		for (unsigned int i = 0; i < numIndicies; i++ )
		{
			this->indicies[i] = indicies[i];
		}
	}

	
}

void MUMesh::setBoundingBox( )
{
	this->setBoundingBox( this->GetMeshBounds() );
}

void MUMesh::setBoundingBox( const GEBoundingBox boundingBox)
{
	this->boundingBox = boundingBox;
}


// Getters

unsigned char MUMesh::getMeshType() const
{
	return this->meshType;
}

unsigned int MUMesh::getNumVerts() const
{
	return this->numVerts;
}

const GEVertex* MUMesh::getVerticies() const
{
	return this->vertices;
}

unsigned int MUMesh::getNumIndicies() const
{
	return this->numIndicies;
}

const unsigned int* MUMesh::getIndicies() const
{
	return this->indicies;
}

GEBoundingBox MUMesh::getBoundingBox() const
{
	return this->boundingBox;
}

// Functions
void MUMesh::SetVertexColor( unsigned int vertIndex, GEvec3 color )
{
	// verify that we are in range
	if ( vertIndex < numVerts )
	{
		vertices[ vertIndex ].r = color.r;
		vertices[ vertIndex ].g = color.g;
		vertices[ vertIndex ].b = color.b;
	}
}

GEBoundingBox MUMesh::GetMeshBounds() const
{
	GEBoundingBox meshBounds;
	
	// verify that there is a mesh.
	if( this->vertices != nullptr )
	{
		

		// initialize all the max/mins to the first vertex.
		GEvec3 meshMin;
		GEvec3 meshMax;

		meshMin.x = meshMax.x = this->vertices[0].x;
		meshMin.y = meshMax.y = this->vertices[0].y;
		meshMin.z = meshMax.z = this->vertices[0].z;

		// find the min/max value for each axis
		for( unsigned int i = 1; i < this->getNumVerts(); i++ )
		{
			meshMin.x = glm::min( meshMin.x, this->vertices[i].x );
			meshMax.x = glm::max( meshMax.x, this->vertices[i].x );

			meshMin.y = glm::min( meshMin.y, this->vertices[i].y );
			meshMax.y = glm::max( meshMax.y, this->vertices[i].y );

			meshMin.z = glm::min( meshMin.z, this->vertices[i].z );
			meshMax.z = glm::max( meshMax.z, this->vertices[i].z );	
		}

		meshBounds.setMin( meshMin );
		meshBounds.setMax( meshMax );
	}

	return meshBounds;
}
