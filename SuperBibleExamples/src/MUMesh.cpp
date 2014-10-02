
#include <cstdlib>
#include <cstring>
#include <algorithm>

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
}

MUMesh::~MUMesh()
{
	if ( this->getVerticies() != nullptr )
	{
		delete[] this->vertices;
		this->vertices = nullptr;
	}
	if ( this->getIndicies() != nullptr )
	{
		delete[] this->indicies;
		this->indicies = nullptr;
	}
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

	if (this->vertices != nullptr )
		delete[] vertices;

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
}

void MUMesh::setIndicies( const unsigned int numIndicies, const unsigned int* indicies )
{
	this->numIndicies = numIndicies;

	if (this->indicies != nullptr )
		delete[] indicies;

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
