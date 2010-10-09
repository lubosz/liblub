#version 330 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec2 uv;
out vec2 uv2;

void main()
{
    gl_Position    = MVPMatrix * vec4(in_Vertex,1);

	uv = in_Uv;
	
	vec3 u = normalize( (MVMatrix * vec4(in_Vertex,1)).xyz );
	vec3 n = normalize( NormalMatrix * in_Normal);
	vec3 r = reflect( u, n );
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	uv2.x = r.x/m + 0.5;
	uv2.y = r.y/m + 0.5;
}

