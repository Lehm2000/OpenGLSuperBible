#version 430 core

layout (quads, fractional_odd_spacing, ccw) in;

uniform sampler2D displaceTexture;
uniform mat4 world_matrix;
uniform mat4 view_matrix;

void main(void)
{
	vec4 p1 = mix( gl_in[0].gl_Position, gl_in[1].gl_Position, gl_TessCoord.x );
	vec4 p2 = mix( gl_in[2].gl_Position, gl_in[3].gl_Position, gl_TessCoord.x );
	vec4 tempPos =  mix( p1, p2, gl_TessCoord.y );
	vec4 tempColor = texture( displaceTexture, vec2((tempPos.x +1.0)/2.0,(tempPos.z +1.0)/2.0));
	//vec4 tempColor = texture( displaceTexture, vec2(tempPos.x ,tempPos.z));

	gl_Position = view_matrix * world_matrix * vec4(tempPos.x, tempPos.y+tempColor.r, tempPos.z, tempPos.w);
	//gl_Position = tempPos;
}