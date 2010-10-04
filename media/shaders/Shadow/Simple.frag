#version 330 core

in vec4 positionCamView;

out vec4 fragColor;

uniform sampler2DShadow shadowMap;

uniform vec3 lightPositionView;

uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

void main() 
{ 
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionCamView;

	float shadowMapDepth = textureProj(shadowMap, shadowTexCoord);
	
	fragColor = vec4(1,1,1,1)*shadowMapDepth;
} 

