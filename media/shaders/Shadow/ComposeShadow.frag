#version 330 core

in vec4 positionCamView;
in vec3 normalCamView;
in vec2 uv;

out vec4 fragColor;


//uniform sampler2DShadow shadowMap;

uniform vec3 lightPositionView;
uniform sampler2D shadowMap;
uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

void main() 
{ 
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionCamView;

	//float shadowMapDepth = textureProj(shadowMap, shadowTexCoord);
	vec4 shadowMapDepth = textureProj(shadowMap, shadowTexCoord);
	
	fragColor = vec4(1,1,1,1)-shadowMapDepth;

} 
