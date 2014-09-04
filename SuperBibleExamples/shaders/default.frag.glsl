#version 430 core

out vec4 color;

//vec4 myColor();	// debug test.

in VS_OUT
{
	//vec2 tc;
	vec4 color;
} fs_in;


void main(void)
{
	color = fs_in.color;
	//color = myColor();		//debug link test.
}