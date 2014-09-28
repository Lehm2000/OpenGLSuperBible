#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 tc;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

out VS_OUT
{
	vec4 color; 
	vec3 normal;
	vec2 tc;
} vs_out;



void main(void)
{
	
	gl_Position =  viewMatrix * worldMatrix * position;
	vs_out.color = color;
	vs_out.normal = normal;
	vs_out.tc = tc;
}