#version 430 core

out vec4 color;

uniform sampler2D texture1;

in VS_OUT
{
	vec4 color; 
	vec3 normal;
	vec2 tc;
} gs_in;

void main(void)
{
	
	color = texture( texture1, gs_in.tc );
	
}