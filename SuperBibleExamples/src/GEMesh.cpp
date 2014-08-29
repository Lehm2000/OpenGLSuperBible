


#include "GEMesh.h"

// Structors
GEMesh::GEMesh()
{
}

GEMesh::GEMesh( GLenum meshType, unsigned int numIndices, GLuint voa, GLuint vb )
{
	this->setMeshType( meshType );
	this->setNumIndices( numIndices );
	this->setVOA( voa );
	this->setVB( vb );
}

GEMesh::~GEMesh()
{
}

// Setters
void GEMesh::setMeshType( const GLenum meshType )
{
	this->meshType = meshType;
}

void GEMesh::setNumIndices( const unsigned int numIndices )
{
	this->numIndices = numIndices;
}

void GEMesh::setVOA( const GLuint voa )
{
	this->voa = voa;
}

void GEMesh::setVB( const GLuint vb )
{
	this->vb = vb;
}


//Getters
GLenum GEMesh::getMeshType() const
{
	return this->meshType;
}

unsigned int GEMesh::getNumIndices() const
{
	return this->numIndices;
}

GLuint GEMesh::getVOA() const
{
	return this->voa;
}

GLuint GEMesh::getVB() const
{
	return this->vb;
}