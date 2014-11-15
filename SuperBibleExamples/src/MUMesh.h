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
#include "GEBoundingBox.h"

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
	unsigned int* indicies;		// Index data. These define the triangles are indexes to the vertices
	GEBoundingBox boundingBox;	// Bounding box that encompasses the entire mesh 

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
	void setBoundingBox( );																// uses the mesh data to calculate the correct bounding box
	void setBoundingBox( const GEBoundingBox boundingBox);								// set arbitrary bounding box.

	// Getters

	unsigned char getMeshType() const;
	unsigned int getNumVerts() const;
	const GEVertex* getVerticies() const;
	unsigned int getNumIndicies() const;
	const unsigned int* getIndicies() const;

	/**
		getBoundingBox()
		Returns the current bounding box applied to the mesh.
	*/
	GEBoundingBox getBoundingBox() const;

	// Functions
	void SetVertexColor( unsigned int vertIndex, GEvec3 color );

	/**
		GetBounds()
		Returns a bounding box coresponding to the meshes size.
	*/
	GEBoundingBox GetMeshBounds() const;
};



#endif /* MUMESH_H */