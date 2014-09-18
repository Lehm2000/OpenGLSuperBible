#version 430 core

out vec4 color;

in VS_OUT
{
	vec4 color;
	vec4 normal;
} fs_in;

void main(void)
{
	
	color = vec4(0.5, 0.5, 1.0, 1.0);
}