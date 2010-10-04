#version 330 core

in vec4 positionCamView;
in vec3 normalCamView;
in vec2 uv;

out vec4 fragColor;

//uniform vec3 lightColor;

//uniform sampler2D colorTex;
uniform sampler2D shadowMap;
//uniform sampler2DShadow shadowMap;

uniform vec3 lightPositionView;

uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

void main() 
{ 

	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionCamView; 
	//correct coord by devision by w:
	shadowTexCoord = shadowTexCoord / shadowTexCoord.w;


	float fragDepth = shadowTexCoord.z;
	
	float shadowMapDepth = texture(shadowMap, shadowTexCoord.xy).z;
	//float shadowMapDepth = textureProj(shadowMap, shadowTexCoord);
	
	// comparez1111
	float receivedLightAmount;
	if(fragDepth > shadowMapDepth )
	{
		 //zmofg lit!
		 receivedLightAmount = 1.0;
 	}
 	else
 	{
 		//zmofgShadowed!!11
 		receivedLightAmount = 0.3; //not exactly 0 because of a faked ambient term
 	}
 
 

 	
 	// TODO zmofg phongz light laelz
 	//FragColor    = texture(colorTex, uv)* lightColor;
 	fragColor = vec4(1,1,1,1);
	fragColor *= receivedLightAmount;
	//fragColor *= shadowMapDepth;
	

} 
