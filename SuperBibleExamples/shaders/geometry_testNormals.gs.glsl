#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices=3) out;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

out VS_OUT
{
	vec4 color; 
	vec3 normal;
	vec2 tc;
} vs_in[3];

out GS_OUT
{
	vec4 color; 
	vec3 normal;
	vec2 tc;
} gs_out;

void main(void)
{
	
	vec3 ab = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 ac = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;

	vec3 faceNormal = normalize( cross( ab, ac ) );

	vec3 transformedFaceNormal = ( viewMatrix * worldMatrix * vec4( faceNormal, 0.0 ) ).xyz;

	for ( int i = 0; i<3; i++ )
	{
		gs_out.color = vec4( ( transformedFaceNormal.x + 1.0 ) / 2.0 , ( transformedFaceNormal.y + 1.0 ) / 2.0, ( transformedFaceNormal.z *-1.0 + 1.0 ) / 2.0 , 1.0 );
		gs_out.normal = vs_in[i].normal;
		gs_out.tc = vs_in[i].tc;
		gl_Position= viewMatrix * worldMatrix * gl_in[i].gl_Position;
		EmitVertex();
	}
	EndPrimitive();
	
}