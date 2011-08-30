{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
struct LightSource {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 direction; 
};

out vec4 finalTarget;
out vec4 finalSpecularTarget;
out vec4 finalDiffuseTarget;
out vec4 envTarget;
{% endblock %}

{% block uniforms %}
uniform sampler2D positionTarget;
uniform sampler2D normalTarget;
uniform sampler2D diffuseTarget;
uniform sampler2D tangentTarget;
uniform sampler2D normalMapTarget;
uniform sampler2D finalAOTarget;
uniform samplerCube envMap;
//uniform sampler2D shadowTarget;

uniform LightSourceBuffer {
	LightSource lightSources[5];
};

uniform vec4 camPosition;

const int shininess = 32;
//const int shininess = 1;

float saturate(float input) {
	return clamp(input, 0.0, 1.0); 
}

{% endblock %}

{% block main %}
	vec4 position = texture(positionTarget, uv);
	vec4 normal = texture(normalTarget, uv);
	vec4 tangent = texture(tangentTarget, uv);
	vec3 binormal = cross(tangent.xyz, normal.xyz);
	vec4 diffuse = texture(diffuseTarget, uv);
	
	//normalmapping
	vec3 eyeVec = vec3(
		dot(-position.xyz, tangent.xyz),
		dot(-position.xyz, binormal.xyz),
		dot(-position.xyz, normal.xyz)
	);
	vec3 bump = normalize( texture(normalMapTarget, uv).xyz * 2.0 - 1.0);
	//endnormalmapping

	//env
	vec3 reflectDir = reflect(-position.xyz, normal.xyz);
	envTarget = texture(envMap, reflectDir);

	float ambient = texture(finalAOTarget, uv).r;
	finalSpecularTarget = vec4(0);
	finalDiffuseTarget = vec4(0);
	
	for(int i = 0; i < 5 ; i++) {
		vec4 lightDirection = lightSources[i].position - position;
		//normalmapping
		vec3 lightVec = normalize(vec3(
			dot(lightDirection.xyz, tangent.xyz),
			dot(lightDirection.xyz, binormal.xyz),
			dot(lightDirection.xyz, normal.xyz)
		));
		float diffuseBump = max( dot(lightVec, bump), 0.0 );

		//endnormalmapping
		vec4 L = normalize(lightDirection);	
		vec4 N = normalize(normal);
		float lambertTerm = max( dot(N,L), 0.0);
		finalDiffuseTarget += diffuse * lambertTerm * lightSources[i].diffuse * diffuseBump;
		
		//vec4 viewDirection = camPosition - position;
		//vec4 E = normalize(-viewDirection);
		//vec4 R = reflect(-L, N);

		//float specular = pow( max(dot(R, E), 0.0), shininess );
				float specular = pow(clamp(dot(
								reflect(-lightVec, bump), 
								normalize(eyeVec)
							), 
							0.0, 
							1.0
						), shininess );
		
		
		//fragColor = lambertTerm * vec4(1);
		finalSpecularTarget += ((lightSources[i].diffuse + envTarget) / 2) *  specular;
		//fragColor = R;
				
		
		
		
	}
	finalTarget = (finalDiffuseTarget + finalSpecularTarget) * ambient;
	//fragColor *= texture(diffuseTarget, uv)* texture(envTarget, uv);
	//fragColor *= 1-ambient;// * texture(shadowTarget, uv).x;
	//fragColor = vec4(foo/5);
	
	//fragColor = texture(finalAOTarget, uv);
{% endblock %}
