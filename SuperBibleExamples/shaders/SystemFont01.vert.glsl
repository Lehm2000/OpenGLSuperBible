#version 430 core

layout (location = 0) in uint charCode;
uniform float charSize;
uniform vec2 startPos;

out VS_OUT
{
	vec2 tc;
} vs_out;

uniform mat4 screenMatrix;

void main(void)
{
	uint charOffset = gl_InstanceID;
	
	float charWidthMulti = 0.525;

	float uvSize = 0.0625;  // 16 characters per line = 1/16

	const vec2[4] position = vec2[4] ( vec2( 0.0, 0.0 ), vec2( charSize * charWidthMulti, 0.0 ), vec2( 0.0,charSize), vec2( charSize * charWidthMulti, charSize ) );
	const vec2[4] tc = vec2[4] ( vec2( 0.0, 0.0 ), vec2( uvSize * charWidthMulti, 0.0 ), vec2( 0.0, uvSize), vec2( uvSize * charWidthMulti, uvSize ) );

	uint charCode2 = 67;

	uint xCharPos = charCode % 16;
	uint yCharPos = charCode / 16; 
	
	gl_Position = screenMatrix * vec4( position[gl_VertexID].x + ( charSize * float( charOffset ) * charWidthMulti ) + startPos.x, position[gl_VertexID].y + startPos.y, 0.5, 1.0 ) ;
	
	vs_out.tc.x = tc[gl_VertexID].x + ( float(xCharPos) * uvSize);
	vs_out.tc.y = tc[gl_VertexID].y + ( float(yCharPos) * uvSize);
	
}