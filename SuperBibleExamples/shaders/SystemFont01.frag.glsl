#version 430 core

layout (location = 0) out vec4 color;

in VS_OUT
{
	vec2 tc;
} fs_in;

uniform sampler2D fontTexture;

void main (void)
{
	color = texture( fontTexture, fs_in.tc) * vec4( 1.0, 0.0, 0.0, 1.0) ;
	//color = vec4( 1.0, 1.0, 1.0, 1.0 );
}