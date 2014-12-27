#version 430 core

out vec4 color;

vec4 myColor( vec4 inColor);

uniform sampler2D texture1;


in VS_OUT
{
	vec4 color;
	vec3 normal;
	vec2 tc;
} fs_in;

void main(void)
{
	color = texture( texture1, fs_in.tc );
}