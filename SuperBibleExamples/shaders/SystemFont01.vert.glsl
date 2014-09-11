#version 430 core

uniform float charSize;
uniform uint charCode;
uniform uint charOffset;
uniform vec2 startPos;
//in float charSize;
//layout (location = 2) in int charOffset;
//layout (location = 3) in float windowAspect;

out VS_OUT
{
	vec2 tc;
} vs_out;

uniform mat4 screenMatrix;

void main(void)
{
	float charWidthMulit = 0.6;

	float uvSize = 0.0625;  // 16 characters per line = 1/16

	const vec2[4] position = vec2[4] ( vec2( 0.0, 0.0 ), vec2( charSize * charWidthMulit, 0.0 ), vec2( 0.0,charSize), vec2( charSize* charWidthMulit, charSize ) );
	const vec2[4] tc = vec2[4] ( vec2( 0.0, 0.0 ), vec2( uvSize *charWidthMulit, 0 ), vec2( 0, uvSize), vec2( uvSize *charWidthMulit, uvSize ) );

	

	uint xCharPos = charCode % 16;
	uint yCharPos = charCode / 16; 

	//vs_out.tc.x = float(position[gl_VertexID].x * xCharPos) * uvSize;
	//vs_out.tc.y = float(position[gl_VertexID].y * yCharPos) * uvSize;
	
	//gl_Position = vec4( ( position[gl_VertexID].x * float(charOffset) ), position[gl_VertexID].y, 0.5, 1.0 );
	gl_Position = screenMatrix * vec4( position[gl_VertexID].x + ( charSize * float( charOffset ) * charWidthMulit ) + startPos.x, position[gl_VertexID].y + startPos.y, 0.5, 1.0 ) ;
	
	vs_out.tc.x = tc[gl_VertexID].x + ( float(xCharPos) * uvSize);
	vs_out.tc.y = tc[gl_VertexID].y + ( float(yCharPos) * uvSize);
	
}