#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tc;


out VS_OUT
{
	vec4 color; 
	vec3 normal;
} vs_out;



void main(void)
{
	vec4 plane[4] = { vec4( -1.0, -0.5, -1.0, 1.0 ), 
		vec4( 1.0, -0.5, -1.0, 1.0 ), 
		vec4( -1.0, -0.5, 1.0, 1.0 ), 
		vec4( 1.0, -0.5, 1.0, 1.0 ), 
		};

	vec3 normals[4] = { normalize(vec3( -1.0, 1.0, -1.0)),
		normalize(vec3( 1.0, 1.0, -1.0)),
		normalize(vec3( -1.0, 1.0, 1.0)),
		normalize(vec3( 1.0, 1.0, 1.0)) };


	gl_Position =  position;
	vs_out.normal = normal;
	vs_out.color = color;
	
	
}