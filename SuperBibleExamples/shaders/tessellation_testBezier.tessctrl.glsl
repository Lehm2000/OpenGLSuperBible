#version 430 core

layout (vertices = 4) out;

uniform float tessLevel;

in VS_OUT
{
	vec4 color; 
	vec3 normal;
} tcs_in[];

out TCS_OUT
{
	vec4 color; 
	vec3 normal;
} tcs_out[];

void main(void)
{
	if (gl_InvocationID == 0)
	{
		gl_TessLevelInner[0] = tessLevel;
		gl_TessLevelInner[1] = tessLevel;
		gl_TessLevelOuter[0] = tessLevel;
		gl_TessLevelOuter[1] = tessLevel;
		gl_TessLevelOuter[2] = tessLevel;
		gl_TessLevelOuter[3] = tessLevel;
	}

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
	tcs_out[gl_InvocationID].color = tcs_in[gl_InvocationID].color;
	tcs_out[gl_InvocationID].normal = tcs_in[gl_InvocationID].normal;
}