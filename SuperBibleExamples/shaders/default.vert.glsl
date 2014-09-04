#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out VS_OUT
{
	//vec2 tc;
	vec4 color;  // removed for debug test
	//color;			// debug version
} vs_out;

uniform mat4 world_matrix;
uniform mat4 view_matrix;

void main(void)
{
	gl_Position = view_matrix * world_matrix * position;
	vs_out.color = color;
	//vs_out.tc = tc;
}