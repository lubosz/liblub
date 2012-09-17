#version {{ version }}

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
	
	vec3 lVec = lightVec * inversesqrt(dot(lightVec, lightVec));
	vec3 bump = normalize( texture(normalMap, uv).xyz * 2.0 - 1.0);

	float diffuseBump = max( dot(lVec, bump), 0.0 );
	
	vec4 diffuseColor = texture(colorMap, uv);
	
	finalColor = 
		lightColor 
		* diffuseBump
		* diffuseColor;	

	float specular = pow(
						clamp(
							dot(
								reflect(-lVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), 
						shininess 
					);
	
	finalColor += 
		lightColor 
		* specularMaterialColor 
		* specular;	
	
}


