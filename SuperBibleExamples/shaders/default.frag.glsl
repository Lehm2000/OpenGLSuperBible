#version 430 core

out vec4 color;

vec4 myColor( vec4 inColor);

in VS_OUT
{
	vec4 color;
} fs_in;



void main(void)
{
	color = myColor( fs_in.color );	
}