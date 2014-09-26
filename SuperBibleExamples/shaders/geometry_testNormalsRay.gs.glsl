#version 430 core

layout (triangles) in;
layout (line_strip, max_vertices=2) out;

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
	float normalVisLength = 0.05;  // how long the normal should appear.
	
	vec3 ab = gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz;
	vec3 ac = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;

	vec3 faceNormal = normalize( cross( ab, ac ) );

	vec3 faceCenter = ( gl_in[0].gl_Position.xyz + gl_in[1].gl_Position.xyz + gl_in[2].gl_Position.xyz ) / 3.0;

	vec3 normalEndPoint = ( faceNormal * normalVisLength ) + faceCenter;
	
	gs_out.color = vec4( 0.5 ,  0.5,  0.5, 1.0 );
	gl_Position= viewMatrix * worldMatrix * vec4( faceCenter, 1.0);
	EmitVertex();

	gs_out.color = vec4( 0.0 ,  1.0,  0.0, 1.0 );
	gl_Position= viewMatrix * worldMatrix * vec4( normalEndPoint, 1.0);
	EmitVertex();
	
	EndPrimitive();
	
}