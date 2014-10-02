#ifndef MESHUTILITIES_H
#define MESHUTILITIES_H

/**
	MeshUtilities
	Holds methods for loading/saving/modifying meshes.
	Author: Jeff Adams
*/

#include <string>

#include "MUMesh.h"

class MeshUtilities
{
private:
public:

	// Functions

	/**
		FillGEVertex
		Fills in all the values for a GEVertex;
	*/
	void FillGEVertex( GEVertex* dest, float x, float y, float z, float r, float g, float b, float a, float nx, float ny, float nz, float u, float v );

	/**
		LoadASE()
		Loads a mesh from the Autodesk ASCII Scene Export format.
		@param meshPath - location of the mesh to be loaded.
		@return the loaded mesh.
	*/
	MUMesh LoadASE( std::string meshPath );

	/**
		ColorizeMesh()
		Converts the xyz values to rgb values.  
		@param mesh - mesh to colorize
	*/
	void ColorizeMesh( MUMesh* mesh );
};

/**
	
*/
bool operator==( const GEVertex& vert1, const GEVertex& vert2);


#endif /* MESHUTILITIES_H */