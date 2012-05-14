{% extends "base.frag" %}

{% block uniforms %}
uniform sampler2D NormalMap;
uniform samplerCube EnvironmentMap;

uniform vec4 deepColor;
uniform vec4 shallowColor;
uniform vec4 reflectionColor;

uniform float reflectionAmount;
uniform float reflectionBlur;
uniform float waterAmount;
uniform float fresnelPower;
uniform float fresnelBias;
uniform float hdrMultiplier;

uniform vec4 lightPositionView;
{% endblock %}


{% block linkage %}
in vec2 uv;
in vec3 color;
in vec3 normal;

in mat3 rotMatrix; // first row of the 3x3 transform from tangent to cube space
in vec2 bumpCoord0;
in vec2 bumpCoord1;
in vec2 bumpCoord2;
in vec3 eyeVector;
in vec4 positionView;
{% endblock %}

float shininess = 20.0;

//ambient
uniform vec4 ambientSceneColor;
//specular
uniform float shininess;
uniform vec4 specularMaterialColor;
uniform vec4 diffuseMaterialColor;

vec4 diffuseColor(float lambertTerm){

        return
#ifdef useDiffuseTexture
//                texture(diffuseTexture, uv) *
#endif
//                diffuseMaterialColor *
//                        lightColor *
                vec4(color,1)*
                        lambertTerm;
}

vec4 specularColor(vec3 L, vec3 N){
//                vec3 E = normalize(-positionView.xyz);
//                vec3 R = reflect(-L, N);

//                float specular = pow( max(dot(R, E), 0.0), shininess );

//        return	specularMaterialColor *
////                        lightColor *
//                        specular;
}


{% block main %}
	// sum normal maps
	// sample from 3 different points so no texture repetition is noticeable
    vec4 t0 = texture(NormalMap, bumpCoord0) * 2.0 - 1.0;
    vec4 t1 = texture(NormalMap, bumpCoord1) * 2.0 - 1.0;
    vec4 t2 = texture(NormalMap, bumpCoord2) * 2.0 - 1.0;
    vec3 N = t0.xyz + t1.xyz + t2.xyz;
    //N = normalize(rotMatrix * N);
    N = normalize(N);

	// reflection
	//negate eye vector for propper cubemap lookup
    vec3 E = -normalize(eyeVector);
    vec3 R = reflect(E, N);
    // Ogre conversion for cube map lookup
   // R.z = -R.z;

//    vec4 reflection = vec4(1);
        vec4 reflection = texture(EnvironmentMap, R, reflectionBlur);
    // cheap hdr effect
    reflection.rgb *= (reflection.r + reflection.g + reflection.b) * hdrMultiplier;

	// fresnel
    float facing = 1.0 - dot(-E, N);
    float fresnel = clamp(fresnelBias + pow(facing, fresnelPower), 0.0, 1.0);

    vec4 waterColor = mix(shallowColor, deepColor, facing) * waterAmount;

    reflection = mix(waterColor,  reflection * reflectionColor, fresnel) * reflectionAmount;
        fragColor = vec4(normal,1);
//        fragColor = vec4(N,1);
//	fragColor = vec4(color1,1) + fragColor * vec4(color2,1);

        fragColor = vec4(0);
        vec4 lightDirection = lightPositionView - positionView;
        vec3 L = normalize(lightDirection.xyz);
//        vec3 N = normalize(normal);
        N = normalize(normal);
        float lambertTerm = max( dot(N,L), 0.0);

        if(lambertTerm > 0.0)	{
//            fragColor += diffuseColor(lambertTerm);
            fragColor += vec4(color,1)
                  *lambertTerm;
                vec3 E = normalize(-positionView.xyz);
                vec3 R = reflect(-L, N);

                float specular = pow( max(dot(R, E), 0.0), 20 );

            fragColor += specular * vec4(1,1,1,1);
        }

        fragColor = vec4(color,1);
//        fragColor = vec4(1,0,0,1);
{% endblock %}
