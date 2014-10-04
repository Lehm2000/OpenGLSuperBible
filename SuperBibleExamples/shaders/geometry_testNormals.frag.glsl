#version 430 core

out vec4 color;

in GS_OUT
{
	vec4 color; 
	vec3 normal;
	vec2 tc;
} gs_in;

void main(void)
{
	
	color = gs_in.color;
	//color =  vec4 ( 1.0, 1.0, 1.0, 1.0 );
}