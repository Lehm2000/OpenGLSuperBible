#ifndef GEMATERIAL_H
#define GEMATERIAL_H

/**
	GEShader
	Purpose: Class to hold the (OpenGL) program and texture references for a shader.
	Author: Jeff Adams
*/

#include <vector>
#include <string>
#include <GL\glew.h>

class GEMaterial
{
private:
	GLuint program;					// compiled program
	std::vector<std::string> textures;	// names of textures used in this program (textures will be stored in a map elsewhere)
	
public:
	// Structors

	GEMaterial();
	GEMaterial( GLuint program, std::vector<std::string> textures );
	~GEMaterial();

	// Setters
	
	void setProgram( const GLuint program );
	void setTextures( const std::vector<std::string> textures );

	// Getters

	GLuint getProgram() const;
	std::vector<std::string> getTextures() const;

};


#endif /* GEMATERIAL_H */