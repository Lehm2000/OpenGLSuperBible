#ifndef GEMESH_H
#define GEMESH_H

/**
	GEMesh Class
	Holds the mesh and mesh properties for a single mesh type for the graphics engine
*/

#include <GL\glew.h>

class GEMesh
{
private:
	// Members
	GLenum meshType;	// Type of mesh ie GL_TRIANGLES etc
	unsigned int numIndices;	// Number of points to render... I think.

	GLuint voa;		// Vertex Array Object for the mesh
	GLuint vb;		// Vertex Buffer for the mesh
public:

	// Structors
	GEMesh();
	GEMesh( GLenum meshType, unsigned int numIndices, GLuint voa, GLuint vb );
	~GEMesh();

	// Setters
	void setMeshType( const GLenum meshType );
	void setNumIndices( const unsigned int numIndices );
	void setVOA( const GLuint voa );
	void setVB( const GLuint vb );

	//Getters
	GLenum getMeshType() const;
	unsigned int getNumIndices() const;
	GLuint getVOA() const;
	GLuint getVB() const;

};


#endif /* GEMESH_H */