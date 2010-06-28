#version 150 core

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec3 LightPosition;

uniform float depth;
uniform float texsize;
uniform float csm_gain;
uniform float csm_offset;
uniform float linearAttenuation;
uniform int conesteps;

uniform sampler2D texmap;
//uniform sampler2D heightmap;
uniform sampler2D stepmap;

   
in vec2 texCoord;
in vec3 eyeSpaceVert;
//in vec3 LightPosition;

in vec3 eyeSpaceTangent;
in vec3 eyeSpaceBinormal;
in vec3 eyeSpaceNormal;

out vec4 gl_FragColor;

void main(void) {
	vec3 texColor = vec3(texture(texmap, texCoord));
    gl_FragColor = vec4(texColor,1.0);
}
