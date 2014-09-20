#version 430 core

out vec4 color;

in VS_OUT
{
	vec4 color;
	vec4 normal;
} fs_in;

void main(void)
{
	
	color = vec4(0.5 * ((gl_FragCoord.z /gl_FragCoord.w +2.0) / 4.0), 0.5 * ((gl_FragCoord.z /gl_FragCoord.w+2.0) / 4.0), 1.0 * ((gl_FragCoord.z /gl_FragCoord.w+2.0) / 4.0), 1.0);
}