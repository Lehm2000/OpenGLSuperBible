#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

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


	gl_Position =  plane[gl_VertexID];
	vs_out.color = color;
	vs_out.normal = normals[gl_VertexID];
	
}