#version 330 core

//precision highp float;

in vec4 positionCamView;
in vec3 normalCamView;

out vec4 finalColor;

uniform vec3 LightPosition;



void main() 
{ 
//Ambient
const vec4 ambientMaterial = vec4(0.2,0.2,0.2,1);
const vec4 ambientLightColor = vec4(.3,.5,.9,1);
const vec4 ambientSceneColor = vec4(1,1,1,1);

//Diffuse
const vec4 diffuseMaterialColor = vec4(0,1,0,1);
const vec4 diffuseLightColor = vec4(0,0,1,1);

//Specular
const float shininess = 1;
const vec4 specularMaterialColor = vec4(1,1,1,1);
const vec4 specularLightColor = vec4(1,1,1,1);

vec4 lightDirection = vec4(LightPosition,1) - positionCamView;

//Ambient
finalColor = 
	(ambientSceneColor * ambientMaterial) 
	+ 
	(ambientLightColor * ambientMaterial);
							
	vec3 N = normalize(normalCamView);
	vec3 L = normalize(lightDirection.xyz);
	
	float lambertTerm = dot(N,L);
	
	if(lambertTerm > 0.0)
	{
		//Diffuse
		finalColor +=	diffuseMaterialColor * 
		              	diffuseLightColor * 
					  	lambertTerm;	
		
		//Specular
		vec3 E = normalize(-positionCamView.xyz);
		vec3 R = reflect(-L, N);
		float specular = pow( max(dot(R, E), 0.0), shininess );
		finalColor +=	specularLightColor * 
		             	specularMaterialColor * 
					 	specular;	
	}
	//finalColor = vec4(normalCamView,1);
			
} 

