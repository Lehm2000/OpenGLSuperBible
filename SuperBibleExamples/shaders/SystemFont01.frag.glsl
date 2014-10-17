#version 430 core

layout (location = 0) out vec4 color;

uniform vec4 fontColor;

in VS_OUT
{
	vec2 tc;
} fs_in;

uniform sampler2D fontTexture;

void main (void)
{
	color = texture( fontTexture, fs_in.tc) * fontColor;
	//color = vec4( tcolor.r, tcolor.g, tcolor.b, tcolor.a );
	//color = vec4( 1.0, 1.0, 1.0, 1.0 );
}