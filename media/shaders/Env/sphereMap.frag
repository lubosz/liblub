#version 330 core

uniform sampler2D envMap;

in vec3 r;

out vec4 fragColor;

void main() {
	vec2 uv;
	float m = 2.0 * sqrt( r.x*r.x + r.y*r.y + (r.z+1.0)*(r.z+1.0) );
	uv.x = r.x/m + 0.5;
	uv.y = r.y/m + 0.5;
	
	fragColor = texture( envMap, uv);
}

