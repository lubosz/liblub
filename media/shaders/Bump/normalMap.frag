#version 330 core

in vec3 lightVec;
in vec3 eyeVec;
in vec2 uv;


uniform sampler2D colorMap;
uniform sampler2D normalMap;
uniform float invRadius;

uniform vec4 lightColor;
uniform vec4 diffuseMaterialColor;
uniform float shininess;
uniform vec4 specularMaterialColor;
uniform vec4 ambientSceneColor;

out vec4 finalColor;

void main (void)
{

	float distSqr = dot(lightVec, lightVec);
	float att = clamp(1.0 - invRadius * sqrt(distSqr), 0.0, 1.0);
	vec3 lVec = lightVec * inversesqrt(distSqr);

	vec3 vVec = normalize(eyeVec);
	
	vec4 base = texture(colorMap, uv);
	
	vec3 bump = normalize( texture(normalMap, uv).xyz * 2.0 - 1.0);


	float diffuse = max( dot(lVec, bump), 0.0 );
	
	vec4 vDiffuse = lightColor 
	//* diffuseMaterialColor 
	* diffuse;	

	float specular = pow(clamp(dot(reflect(-lVec, bump), vVec), 0.0, 1.0), shininess );
	
	vec4 vSpecular = lightColor * specularMaterialColor *  specular;	
	
	//finalColor = ( ambientSceneColor + vDiffuse*base + vSpecular) * att;
	finalColor = (vDiffuse*base+ vSpecular);
}


