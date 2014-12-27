#version 430 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec4 color;

out VS_OUT
{
	vec4 color;
} vs_out;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

void main(void)
{
	gl_Position = viewMatrix * worldMatrix * position;
	vs_out.color = color;
}