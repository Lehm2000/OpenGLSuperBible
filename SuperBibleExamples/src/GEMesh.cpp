


#include "GEMesh.h"

// Structors
GEMesh::GEMesh()
{
}

GEMesh::GEMesh( GLenum meshType, unsigned int numVertices, unsigned int numIndices, GLuint vertexArrayObject, GLuint vertexBuffer, GLuint indexBuffer, GLuint indirectBuffer )
{
	this->setMeshType( meshType );
	this->setNumVertices( numVertices );
	this->setNumIndices( numIndices );
	this->setVertexArrayObject( vertexArrayObject );
	this->setVertexBuffer( vertexBuffer );
	this->setIndexBuffer( indexBuffer );
	this->setIndirectBuffer( indirectBuffer );
}

GEMesh::~GEMesh()
{
}

// Setters
void GEMesh::setMeshType( const GLenum meshType )
{
	this->meshType = meshType;
}

void GEMesh::setNumVertices( const unsigned int numVertices)
{
	this->numVertices = numVertices;
}

void GEMesh::setNumIndices( const unsigned int numIndices )
{
	this->numIndices = numIndices;
}

void GEMesh::setVertexArrayObject( const GLuint vertexArrayObject )
{
	this->vertexArrayObject = vertexArrayObject;
}

void GEMesh::setVertexBuffer( const GLuint vertexBuffer )
{
	this->vertexBuffer = vertexBuffer;
}

void GEMesh::setIndexBuffer( const GLuint indexBuffer )
{
	this->indexBuffer = indexBuffer;
}

void GEMesh::setIndirectBuffer( const GLuint indirectBuffer )
{
	this->indirectBuffer = indirectBuffer;
}

//Getters
GLenum GEMesh::getMeshType() const
{
	return this->meshType;
}

unsigned int GEMesh::getNumVertices() const
{
	return this->numVertices;
}

unsigned int GEMesh::getNumIndices() const
{
	return this->numIndices;
}

GLuint GEMesh::getVertexArrayObject() const
{
	return this->vertexArrayObject;
}

GLuint GEMesh::getVertexBuffer() const
{
	return this->vertexBuffer;
}

GLuint GEMesh::getIndexBuffer() const
{
	return this->indexBuffer;
}

GLuint GEMesh:: getIndirectBuffer() const
{
	return this->indirectBuffer;
}

