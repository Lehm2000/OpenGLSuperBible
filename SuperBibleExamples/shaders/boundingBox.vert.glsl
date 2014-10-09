#version 430 core

uniform vec3 bmin;
uniform vec3 bmax;

uniform mat4 worldMatrix;
uniform mat4 viewMatrix;

void main (void)
{
	// start by defining 8 points to represent the corners of the bounding box
	vec3 bBox[] = { 
		vec3( bmin.x, bmin.y, bmin.z),
		vec3( bmin.x, bmin.y, bmax.z ),
		vec3( bmax.x, bmin.y, bmin.z ),
		vec3( bmax.x, bmin.y, bmax.z ),
		vec3( bmax.x, bmax.y, bmin.z ),
		vec3( bmax.x, bmax.y, bmax.z ),
		vec3( bmin.x, bmax.y, bmin.z ),
		vec3( bmin.x, bmax.y, bmax.z )
	};
		
	int vertexList[] = { 
		0,2,
		2,4,
		4,6,
		6,0,
		
		1,3,
		3,5,
		5,7,
		7,1,
		
		0,1,
		2,3,
		4,5,
		6,7
	};
	
	gl_Position = viewMatrix * worldMatrix * vec4( bBox[ vertexList[ gl_VertexID ] ], 1.0 );
}