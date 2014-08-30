


#include "GEShader.h"


// Structors

GEShader::GEShader()
{
	this->setProgram( 0 );  // set it to no program
}

GEShader::GEShader( GLuint program, std::vector<std::string> textures )
{
	this->setProgram( program );
	this->setTextures( textures );
}

GEShader::~GEShader()
{
}

// Setters
	
void GEShader::setProgram( const GLuint program )
{
	this->program = program;
}

void GEShader::setTextures( const std::vector<std::string> textures )
{
	this->textures = textures;
}

// Getters

GLuint GEShader::getProgram() const
{
	return this->program;
}

std::vector<std::string> GEShader::getTextures() const
{
	return this->textures;
}