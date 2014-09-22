#ifndef GEMESH_H
#define GEMESH_H

/**
	GEMesh Class
	Holds the mesh and mesh properties for a single mesh type for the graphics engine (this is the openGL version)
*/

#include <GL\glew.h>


typedef struct
{
	GLuint elemCount;
	GLuint instanceCount;
	GLuint firstIndex;
	GLint baseVertex;
	GLuint baseInstance;
} DrawElementsIndirectCommand;

class GEMesh
{
private:
	// Members
	GLenum meshType;	// Type of mesh ie GL_TRIANGLES etc
	unsigned int numVertices;	// Number of vertices in the mesh
	unsigned int numIndices;	// Number of points to render... I think.

	GLuint vertexArrayObject;	// Vertex Array Object for the mesh
	GLuint vertexBuffer;		// Vertex Buffer for the mesh
	GLuint indexBuffer;			// Index Buffer for the mesh
	GLuint indirectBuffer;		// Indirect Buffer	

public:

	// Structors
	GEMesh();
	GEMesh( GLenum meshType, unsigned int numVertices, unsigned int numIndices, GLuint vertexArrayObject, GLuint vertexBuffer, GLuint indexBuffer, GLuint indirectBuffer );
	~GEMesh();

	// Setters
	void setMeshType( const GLenum meshType );
	void setNumVertices( const unsigned int numVertices );
	void setNumIndices( const unsigned int numIndices );
	void setVertexArrayObject( const GLuint voa );
	void setVertexBuffer( const GLuint vb );
	void setIndexBuffer( const GLuint indexBuffer );
	void setIndirectBuffer( const GLuint indirectBuffer );

	//Getters
	GLenum getMeshType() const;
	unsigned int getNumVertices() const;
	unsigned int getNumIndices() const;
	GLuint getVertexArrayObject() const;
	GLuint getVertexBuffer() const;
	GLuint getIndexBuffer() const;
	GLuint getIndirectBuffer() const;

};


#endif /* GEMESH_H */