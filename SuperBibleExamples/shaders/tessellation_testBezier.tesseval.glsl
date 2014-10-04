#version 430 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

in TCS_OUT
{
	vec4 color; 
	vec3 normal;
	float edgeLengths[4];
	vec3 vertexHandle[8];
} tcs_in[];

vec3 BezierPosition( vec3 p1, vec3 p2, vec3 p3, vec3 p4, float dis)
{
	return p1 * pow( 1.0 - dis, 3.0 )
		+ 3.0 * p2 * pow( 1.0 - dis, 2.0 ) * dis
		+ 3.0 * p3 * (1.0 - dis) * pow( dis, 2.0 )
		+ p4 * pow( dis, 3.0 );
}

mat4 rotationMatrix(vec3 axis, float angle)
{
	// Credit: Neil Mendoza
	// http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/

	axis = normalize(axis);
	float s = sin(angle);
	float c = cos(angle);
	float oc = 1.0 - c;
	
	return mat4(oc * axis.x * axis.x + c, oc * axis.x * axis.y - axis.z * s, oc * axis.z * axis.x + axis.y * s, 0.0,
		oc * axis.x * axis.y + axis.z * s, oc * axis.y * axis.y + c, oc * axis.y * axis.z - axis.x * s, 0.0,
		oc * axis.z * axis.x - axis.y * s, oc * axis.y * axis.z + axis.x * s, oc * axis.z * axis.z + c, 0.0,
		0.0, 0.0, 0.0, 1.0);
}

void main(void)
{
	float handleLen = 0.38;  // how long to make the control handles... make uniform??
	
	float edgeLength1 = tcs_in[0].edgeLengths[0];
	float edgeLength2 = tcs_in[0].edgeLengths[1];
	//float edgeLength3 = tcs_in[0].edgeLengths[2];
	//float edgeLength4 = tcs_in[0].edgeLengths[3];

	vec3 handleVector1a = tcs_in[0].vertexHandle[0];
	vec3 handleVector1b = tcs_in[1].vertexHandle[1];
	vec3 handleVector2a = tcs_in[1].vertexHandle[0];
	vec3 handleVector2b = tcs_in[2].vertexHandle[1];
	vec3 handleVector3a = tcs_in[2].vertexHandle[0];
	vec3 handleVector3b = tcs_in[3].vertexHandle[1];
	vec3 handleVector4a = tcs_in[3].vertexHandle[0];
	vec3 handleVector4b = tcs_in[0].vertexHandle[1];

	vec3 p1 = BezierPosition( gl_in[0].gl_Position.xyz,
		gl_in[0].gl_Position.xyz + ( handleVector1a * (edgeLength1 * handleLen ) ),
		gl_in[1].gl_Position.xyz + ( handleVector1b * (edgeLength1 * handleLen ) ),
		gl_in[1].gl_Position.xyz, 
		gl_TessCoord.x);

	vec3 p2 = BezierPosition( gl_in[2].gl_Position.xyz,
		gl_in[2].gl_Position.xyz + ( handleVector3a * (edgeLength2 * handleLen ) ),
		gl_in[3].gl_Position.xyz + ( handleVector3b * (edgeLength2 * handleLen ) ),
		gl_in[3].gl_Position.xyz, 
		gl_TessCoord.x);
	
	vec3 newEdge = p2 - p1;
	float newEdgeLength = length( newEdge );

	vec3 newNormal1 = normalize( mix( tcs_in[0].normal, tcs_in[1].normal, gl_TessCoord.x ) );
	vec3 newNormal2 = normalize( mix( tcs_in[2].normal, tcs_in[3].normal, gl_TessCoord.x ) );

	// get the new bezier handles
	vec3 newEdgeHandle1 = normalize( cross( cross( newNormal1, newEdge), newNormal1 ) )  ;
	vec3 newEdgeHandle2 = normalize( cross( cross( newNormal2, newEdge * -1.0), newNormal2 ) ) ;

	// calculate the position on the bezier curve

	vec4 tempPos = vec4(BezierPosition( p1,
		p1 + (newEdgeHandle1 * (newEdgeLength * handleLen ) ),
		p2 + (newEdgeHandle2 * (newEdgeLength * handleLen ) ),
		p2,
		gl_TessCoord.y)
		, 1.0);
	
	/*vec4 tempPos = vec4(BezierPosition( p1,
		p1 + mix( handleVector4b * (edgeLength1 * handleLen ), handleVector2a * (edgeLength3 * handleLen ),gl_TessCoord.x),
		p2 + mix( handleVector2b * (edgeLength1 * handleLen ), handleVector4a * (edgeLength3 * handleLen ),gl_TessCoord.x),
		p2,
		gl_TessCoord.y)
		, 1.0);*/

	//vec4 tempPos =  vec4( mix( p1, p2, gl_TessCoord.y ), 1.0 );

	gl_Position = viewMatrix * worldMatrix * tempPos;
}