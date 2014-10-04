#ifndef MUMESH_H
#define MUMESH_H

/**
	MUMesh
	Mesh Utilities Mesh. Class to hold mesh information for the engine.  Most 
	of the time it used as a temporary holder for loaded mesh information that 
	is passed to the graphics engine.  Currently only supports triangle lists.

	Author: Jeff Adams
*/

#include "TypeDefinitions.h"

#define GE_MESH_NONE			0
#define GE_MESH_TRIANGLE_LIST	1
#define GE_MESH_TRIANGLE_STRIP	2

struct GEVertex
{
	// Position
	float x;
	float y;
	float z;
	//float w; // needed?

	// Color
	float r;
	float g;
	float b;
	float a;

	// Normal
	float nx;
	float ny;
	float nz;

	// Texture 
	float u;
	float v;
};

class MUMesh
{
private:
	unsigned char meshType;		// GE_TRIANGLE_LIST, GE_TRIANGLE_STRIP
	unsigned int numVerts;		// How many vertices are in the mesh
	GEVertex* vertices;			// Vertice data
	unsigned int numIndicies;	// Number of indicies.  
	unsigned int* indicies;		// Indice data. These define the triangles are indexes to the vertices

public:
	// Structors

	MUMesh();
	MUMesh( const unsigned char meshType, const unsigned int numVerts, const GEVertex* vertices, const unsigned int numIndicies, const unsigned int* indicies );
	MUMesh( const MUMesh& source );
	~MUMesh();

	// Operators
	
	MUMesh& operator=( MUMesh source );

	// Setters

	void setMeshType( const unsigned char meshType );
	void setVertices( const unsigned int numVerts, const GEVertex* vertices );
	void setIndicies( const unsigned int numIndicies, const unsigned int* indicies );

	// Getters

	unsigned char getMeshType() const;
	unsigned int getNumVerts() const;
	const GEVertex* getVerticies() const;
	unsigned int getNumIndicies() const;
	const unsigned int* getIndicies() const;

	// Functions
	void SetVertexColor( unsigned int vertIndex, GEvec3 color );
};



#endif /* MUMESH_H */