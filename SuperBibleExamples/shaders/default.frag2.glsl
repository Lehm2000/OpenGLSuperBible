
vec4 myColor( vec4 inColor)
{
	return vec4( gl_FragCoord.x/1280.0 , gl_FragCoord.y/720.0, 0.0, 1.0 );
}