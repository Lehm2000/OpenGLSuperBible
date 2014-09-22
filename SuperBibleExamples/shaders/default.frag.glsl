#version 430 core

out vec4 color;

vec4 myColor( vec4 inColor);	// debug test.

in VS_OUT
{
	//vec2 tc;
	vec4 color;
} fs_in;



void main(void)
{
	//color = fs_in.color;
	color = myColor( fs_in.color );		//debug link test.
	//color = gl_FragCoord;
}