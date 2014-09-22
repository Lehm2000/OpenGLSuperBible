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
	float edgeLengths[4];
	vec3 vertexHandle[2];  // 0 = outgoing handle, 1 = incoming
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

	// use the normal information to create calculate control handles

	// first calculate the vectors for the handles... we'll need 8 of them... 2 for each vertex
	
	// start by getting vectors for the edges
	vec3 edgeVector1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 edgeVector2 = gl_in[3].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 edgeVector3 = gl_in[3].gl_Position.xyz - gl_in[2].gl_Position.xyz;
	vec3 edgeVector4 = gl_in[0].gl_Position.xyz - gl_in[2].gl_Position.xyz;

	// get the length of those edges
	float edgeLength1 = length( edgeVector1);
	float edgeLength2 = length( edgeVector2);
	float edgeLength3 = length( edgeVector3);
	float edgeLength4 = length( edgeVector4);

	// computer the vector handles. Cross the vertex normal with edge Vector... then cross that with the vertex normal again.
	if (gl_InvocationID == 0)
	{
		tcs_out[gl_InvocationID].vertexHandle[0] = normalize( cross( cross( tcs_in[0].normal, edgeVector1), tcs_in[0].normal ) )  ;
		tcs_out[gl_InvocationID].vertexHandle[1] = normalize( cross( cross( tcs_in[0].normal, edgeVector4 * -1.0), tcs_in[0].normal ) ) ;
	}
	else if (gl_InvocationID == 1)
	{
		tcs_out[gl_InvocationID].vertexHandle[0] = normalize( cross( cross( tcs_in[1].normal, edgeVector2), tcs_in[1].normal ) ) ;
		tcs_out[gl_InvocationID].vertexHandle[1] = normalize( cross( cross( tcs_in[1].normal, edgeVector1 * -1.0), tcs_in[1].normal ) ) ;
	}
	else if (gl_InvocationID == 2)
	{
		tcs_out[gl_InvocationID].vertexHandle[0] = normalize( cross( cross( tcs_in[2].normal, edgeVector3), tcs_in[2].normal ) ) ;
		tcs_out[gl_InvocationID].vertexHandle[1] = normalize( cross( cross( tcs_in[2].normal, edgeVector2 * -1.0), tcs_in[2].normal ) ) ;
	}
	else if (gl_InvocationID == 3)
	{
		tcs_out[gl_InvocationID].vertexHandle[0] = normalize( cross( cross( tcs_in[3].normal, edgeVector4), tcs_in[3].normal ) ) ;
		tcs_out[gl_InvocationID].vertexHandle[1] = normalize( cross( cross( tcs_in[3].normal, edgeVector3 * -1.0), tcs_in[3].normal ) ) ;
	}

	tcs_out[gl_InvocationID].edgeLengths[0] = edgeLength1;
	tcs_out[gl_InvocationID].edgeLengths[1] = edgeLength2;
	tcs_out[gl_InvocationID].edgeLengths[2] = edgeLength3;
	tcs_out[gl_InvocationID].edgeLengths[3] = edgeLength4;

	tcs_out[gl_InvocationID].color = tcs_in[gl_InvocationID].color;
	tcs_out[gl_InvocationID].normal = tcs_in[gl_InvocationID].normal;

	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;
}