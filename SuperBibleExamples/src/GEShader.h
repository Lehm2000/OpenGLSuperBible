#ifndef GESHADER_H
#define GESHADER_H

/**
	GEShader
	Purpose: Class to hold the (OpenGL) program and texture references for a shader.
	Author: Jeff Adams
*/

#include <vector>
#include <string>
#include <GL\glew.h>

class GEShader
{
private:
	GLuint program;					// compiled program
	std::vector<std::string> textures;	// names of textures used in this program (textures will be stored in a map elsewhere)
	
public:
	// Structors

	GEShader();
	GEShader( GLuint program, std::vector<std::string> textures );
	~GEShader();

	// Setters
	
	void setProgram( const GLuint program );
	void setTextures( const std::vector<std::string> textures );

	// Getters

	GLuint getProgram() const;
	std::vector<std::string> getTextures() const;

};


#endif /* GESHADER_H */