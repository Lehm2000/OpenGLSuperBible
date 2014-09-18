#version 430 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform mat4 world_matrix;
uniform mat4 view_matrix;

in TCS_OUT
{
	vec4 color; 
	vec3 normal;
} tcs_in[];

vec3 BezierPosition( vec3 p1, vec3 p2, vec3 p3, vec3 p4, float dis)
{
	return p1 * pow( 1.0 - dis, 3.0 )
		+ 3.0 * p2 * pow( 1.0 - dis, 2.0 ) * dis
		+ 3.0 * p3 * (1.0 - dis) * pow( dis, 2.0 )
		+ p4 * pow( dis, 3.0 );
}

void main(void)
{
	// use the normal information to create a smoothed surface.

	// first calculate the vectors for the handles... we'll need 8 of them... 2 for each vertex
	
	// start by getting vectors for the edges
	vec3 edgeVector1 = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 edgeVector2 = gl_in[3].gl_Position.xyz - gl_in[1].gl_Position.xyz;
	vec3 edgeVector3 = gl_in[3].gl_Position.xyz - gl_in[2].gl_Position.xyz;
	vec3 edgeVector4 = gl_in[0].gl_Position.xyz - gl_in[2].gl_Position.xyz;

	// now get the handles.  Cross the vertex normal with edge Vector... then cross that with the vertex normal again.
	vec3 handleVector1a = normalize( cross( cross( tcs_in[0].normal, edgeVector1), tcs_in[0].normal ) ) * length( edgeVector1) / 2.0;
	vec3 handleVector1b = normalize( cross( cross( tcs_in[1].normal, edgeVector1 * -1.0), tcs_in[1].normal ) ) * length( edgeVector1) / 2.0;

	vec3 handleVector2a = normalize( cross( cross( tcs_in[1].normal, edgeVector2), tcs_in[1].normal ) ) * length( edgeVector2) / 2.0;
	vec3 handleVector2b = normalize( cross( cross( tcs_in[2].normal, edgeVector2 * -1.0), tcs_in[2].normal ) ) * length( edgeVector2) / 2.0;

	vec3 handleVector3a = normalize( cross( cross( tcs_in[2].normal, edgeVector3), tcs_in[2].normal ) ) * length( edgeVector3) / 2.0;
	vec3 handleVector3b = normalize( cross( cross( tcs_in[3].normal, edgeVector3 * -1.0), tcs_in[3].normal ) ) * length( edgeVector3) / 2.0;

	vec3 handleVector4a = normalize( cross( cross( tcs_in[3].normal, edgeVector4), tcs_in[3].normal ) ) * length( edgeVector4) / 2.0;
	vec3 handleVector4b = normalize( cross( cross( tcs_in[0].normal, edgeVector4 * -1.0), tcs_in[0].normal ) ) * length( edgeVector4) / 2.0;

	vec3 p1 = BezierPosition( gl_in[0].gl_Position.xyz,
		gl_in[0].gl_Position.xyz + handleVector1a,
		gl_in[1].gl_Position.xyz + handleVector1b,
		gl_in[1].gl_Position.xyz, 
		gl_TessCoord.x);

	vec3 p2 = BezierPosition( gl_in[2].gl_Position.xyz,
		gl_in[2].gl_Position.xyz + handleVector3a,
		gl_in[3].gl_Position.xyz + handleVector3b,
		gl_in[3].gl_Position.xyz, 
		gl_TessCoord.x);

	vec4 tempPos = vec4(BezierPosition( p1,
		p1 + mix(handleVector4b,handleVector2a,gl_TessCoord.x),
		p2 + mix(handleVector2b, handleVector4a,gl_TessCoord.x),
		p2,
		gl_TessCoord.y)
		, 1.0);

	//vec4 tempPos =  vec4( mix( p1, p2, gl_TessCoord.y ), 1.0 );

	gl_Position = view_matrix * world_matrix * tempPos;
}