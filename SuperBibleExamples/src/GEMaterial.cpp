


#include "GEMaterial.h"


// Structors

GEMaterial::GEMaterial()
{
	this->setProgram( 0 );  // set it to no program
}

GEMaterial::GEMaterial( GLuint program, std::vector<std::string> textures )
{
	this->setProgram( program );
	this->setTextures( textures );
}

GEMaterial::~GEMaterial()
{
}

// Setters
	
void GEMaterial::setProgram( const GLuint program )
{
	this->program = program;
}

void GEMaterial::setTextures( const std::vector<std::string> textures )
{
	this->textures = textures;
}

// Getters

GLuint GEMaterial::getProgram() const
{
	return this->program;
}

std::vector<std::string> GEMaterial::getTextures() const
{
	return this->textures;
}