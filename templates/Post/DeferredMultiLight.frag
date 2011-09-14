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
//uniform sampler2D diffuseTarget;
uniform sampler2D tangentTarget;
uniform sampler2D binormalTarget;
//uniform sampler2D normalMapTarget;
uniform sampler2D finalAOTarget;
uniform samplerCube envMap;
uniform sampler2D uvTarget;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

uniform LightSourceBuffer {
	LightSource lightSources[LIGHTS];
};

uniform vec3 camPositionWorld;

const int shininess = 1;
//const int shininess = 1;

{% endblock %}

{% block main %}
    vec2 uvModel = texture(uvTarget, uv).xy;
	vec3 position = texture(positionTarget, uv).xyz;
	vec3 normal = normalize(texture(normalTarget, uv).xyz);
	vec3 tangent = normalize(texture(tangentTarget, uv).xyz);
	//vec3 binormal = cross(tangent, normal);
	vec3 binormal = normalize(texture(binormalTarget, uv).xyz);

	vec3 viewDirection = normalize(position - camPositionWorld);
	vec3 viewDirectionTS = normalize(vec3(
		dot(viewDirection, tangent),
		dot(viewDirection, binormal),
		dot(viewDirection, normal)
	));
	
	vec4 normalMap = texture(normalTexture, uvModel);
	vec3 normalTS = normalize( normalMap.xyz * 2.0 - 1.0);
	
	{% if paralaxMap %}
	float height = normalMap.w;
	float uvTrans = max( dot(normalTS,viewDirectionTS), 0.0 ) * height;
    vec2 uv2 = uvModel + vec2(uvTrans);
    normalMap = texture(normalTexture, uv2);
    normalTS = normalize( normalMap.xyz * 2.0 - 1.0);
    //height = normalMap.w;
    {% else %}
    vec2 uv2 = uvModel;
    {% endif %}
	vec4 diffuse = texture(diffuseTexture, uv2);
	finalSpecularTarget = vec4(0);
	finalDiffuseTarget = vec4(0);
	
        //vec3 reflectView = reflect(viewDirection, normal);
        //envTarget = texture(envMap, -reflectView);
        vec3 reflectView = reflect(viewDirectionTS, normalTS);
        envTarget = texture(envMap, reflectView);

		float ambient = texture(finalAOTarget, uv).r;
	
	   for(int i = 0; i < LIGHTS ; i++) {
			vec3 lightDirection = normalize(position - lightSources[i].position.xyz);
			vec3 lightDirectionTS = normalize(vec3(
				dot(lightDirection, tangent),
				dot(lightDirection, binormal),
				dot(lightDirection, normal)
			));
			float lambertTermBump = max( dot(normalTS,-lightDirectionTS), 0.0 );
			//float lambertTerm = max( dot(vec4(normal,0),vec4(-lightDirection,0)), 0.0);
		    finalDiffuseTarget += diffuse * lambertTermBump * lightSources[i].diffuse;
	
			//vec3 reflectLight = reflect(lightDirection, normal);
			//float specular = max(dot(reflectLight, -viewDirection), 0.0);
			
			vec3 reflectLight = reflect(lightDirectionTS, normalTS);
			float specular = max(dot(reflectLight, -viewDirectionTS ), 0.0);
			
			specular = pow( specular, shininess );
			
			finalSpecularTarget += lightSources[i].diffuse *  specular;
	   }
	  finalTarget = (finalDiffuseTarget + finalSpecularTarget + envTarget / 3.0)*ambient;
	  //finalTarget = vec4(height);
	  //finalDiffuseTarget = vec4(binormal,1);
	  //finalSpecularTarget = vec4(tangent,1);
	  //finalTarget = texture(uvTarget, uv);
{% endblock %}
