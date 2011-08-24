{% extends "base.frag" %}

{% block linkage %}
in vec2 uv;
struct LightSource {
	vec4 position;
	vec4 diffuse;
	vec4 specular;
	vec4 direction; 
};
{% endblock %}

{% block uniforms %}
uniform sampler2D positionTarget;
uniform sampler2D normalTarget;
uniform sampler2D diffuseTarget;
uniform sampler2D tangentTarget;
uniform sampler2D normalMapTarget;
uniform sampler2D envTarget;
uniform sampler2D depthTarget;
uniform LightSourceBuffer {
	LightSource lightSources[5];
};

uniform vec4 camPosition;

const int shininess = 32;

float saturate(float input) {
	return clamp(input, 0.0, 1.0); 
}


{% endblock %}


{% block main %}
	fragColor = vec4(0);
	for(int i = 0; i < 5 ; i++) {
		vec4 lightDirection = lightSources[i].position - texture(positionTarget, uv);
		vec4 L = normalize(lightDirection);	
		vec4 N = normalize(texture(normalTarget, uv));
		float lambertTerm = max( dot(N,L), 0.0);
		fragColor += lambertTerm * lightSources[i].diffuse;
		
		vec4 viewDirection = camPosition - texture(positionTarget, uv);
		vec4 E = normalize(-viewDirection);
		vec4 R = reflect(-L, N);

		float specular = pow( max(dot(R, E), 0.0), shininess );
		fragColor = lambertTerm * vec4(1);
		fragColor += specular * vec4(1);
		//fragColor = R;
	}
	fragColor *= texture(diffuseTarget, uv)* texture(envTarget, uv);

	//fragColor = texture(envTarget, uv);

/*
	vec4 diffuseColor = vec4(0);
	vec4 specularColor = vec4(0);
	vec4 normal = texture(normalTarget, uv);
	float diffusePower = 1.0;
	float specularPower = 1.0;
	int specularHardness = 32;

    // FIND THE VECTOR BETWEEN THE 3D POSITION IN SPACE OF THE SURFACE
    vec4 lightDirection = lightSources[1].position - texture(positionTarget, uv);
    vec4 viewDirection = camPosition - texture(positionTarget, uv);

    // GET THE DISTANCE OF THIS VECTOR
    float distance = length(lightDirection); 

    // USES INVERSE SQUARE FOR DISTANCE ATTENUATION
    distance = distance * distance; 

    // NORMALIZE THE VECTOR
    lightDirection = normalize(lightDirection); 

    // INTENSITY OF THE DIFFUSE LIGHT       
    // SATURATE TO KEEP WITHIN THE 0-1 RANGE
    // DOT PRODUCT OF THE LIGHT DIRECTION VECTOR AND THE SURFACE NORMAL
    float i = saturate(dot(lightDirection, normal)); 

    // CALCULATE THE DIFFUSE LIGHT FACTORING IN LIGHT COLOUR, POWER AND THE ATTENUATION
    diffuseColor = i * lightSources[1].diffuse * diffusePower / distance; 

    //CALCULATE THE HALF VECTOR BETWEEN THE LIGHT VECTOR AND THE VIEW VECTOR. THIS IS CHEAPER THAN CALCULATING THE ACTUAL REFLECTIVE VECTOR
    vec4 h = normalize(lightDirection + viewDirection);

    // INTENSITY OF THE SPECULAR LIGHT      
    // DOT PRODUCT OF NORMAL VECTOR AND THE HALF VECTOR TO THE POWER OF THE SPECULAR HARDNESS
    //i = pow(saturate(dot(normal, h)), specularHardness);
    i = pow(dot(normal, h), specularHardness);    

    // CALCULATE THE SPECULAR LIGHT FACTORING IN LIGHT SPECULAR COLOUR, POWER AND THE ATTENUATION
    specularColor = i * lightSources[1].specular * specularPower / distance; 
    
    fragColor = i * vec4(1);
*/
{% endblock %}
