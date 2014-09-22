#version 430 core

subroutine vec4 colorSub( vec4 inColor );

subroutine (colorSub)
vec4 colorSubRed( vec4 inColor )
{
	return vec4( 1.0, 0.0, 0.0, 1.0 );
}

subroutine (colorSub)
vec4 colorSubGreen( vec4 inColor )
{
	return vec4( 0.0, 1.0, 0.0, 1.0 );
}

subroutine (colorSub)
vec4 colorSubBlue( vec4 inColor )
{
	return vec4( 0.0, 0.0, 1.0, 1.0 );
}

subroutine uniform colorSub colorSubUniform;

out vec4 color;

in VS_OUT
{
	//vec2 tc;
	vec4 color;
} fs_in;

void main(void)
{
	color = colorSubUniform( vec4( 1.0 ) );
}