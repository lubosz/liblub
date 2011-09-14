{% extends "base.frag" %}

{% block linkage %}
#define LIGHTS {{ lightCount }}
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

uniform LightSourceBuffer {
	LightSource lightSources[LIGHTS];
};

uniform vec3 camPositionWorld;

const int shininess = 1;
//const int shininess = 1;

{% endblock %}

{% block main %}
	vec3 position = texture(positionTarget, uv).xyz;
	vec3 normal = normalize(texture(normalTarget, uv).xyz);
	vec3 tangent = normalize(texture(tangentTarget, uv).xyz);
	vec3 binormal = normalize(cross(tangent, normal));
	vec4 diffuse = texture(diffuseTarget, uv);

	vec3 viewDirection = normalize(position - camPositionWorld);
	vec3 viewDirectionTS = normalize(vec3(
		dot(viewDirection, tangent),
		dot(viewDirection, binormal),
		dot(viewDirection, normal)
	));
	vec3 normalMap = 
	normalize( texture(normalMapTarget, uv).xyz * 2.0 - 1.0);
	//normalize(texture(normalMapTarget, uv).xyz);

	finalSpecularTarget = vec4(0);
	finalDiffuseTarget = vec4(0);
	
        //vec3 reflectView = reflect(viewDirection, normal);
        //envTarget = texture(envMap, -reflectView);
        vec3 reflectView = reflect(viewDirectionTS, normalMap);
        envTarget = texture(envMap, reflectView);

		float ambient = texture(finalAOTarget, uv).r;
	
	   for(int i = 0; i < LIGHTS ; i++) {
			vec3 lightDirection = normalize(position - lightSources[i].position.xyz);
			vec3 lightDirectionTS = normalize(vec3(
				dot(lightDirection, tangent),
				dot(lightDirection, binormal),
				dot(lightDirection, normal)
			));
			float lambertTermBump = max( dot(normalMap,-lightDirectionTS), 0.0 );
			//float lambertTerm = max( dot(vec4(normal,0),vec4(-lightDirection,0)), 0.0);
		    finalDiffuseTarget += diffuse * lambertTermBump * lightSources[i].diffuse;
	
			//vec3 reflectLight = reflect(lightDirection, normal);
			//float specular = max(dot(reflectLight, -viewDirection), 0.0);
			
			vec3 reflectLight = reflect(lightDirectionTS, normalMap);
			float specular = max(dot(reflectLight, -viewDirectionTS ), 0.0);
			
			specular = pow( specular, shininess );
			
			finalSpecularTarget += lightSources[i].diffuse *  specular;
	   }
	  finalTarget = (finalDiffuseTarget + finalSpecularTarget + envTarget / 3.0)*ambient;
{% endblock %}
