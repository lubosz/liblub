#version {{ version }}

precision highp float;

/*
uniform float MaterialThickness;
uniform vec3 ExtinctionCoefficient; // Will show as X Y and Z ports in QC, but actually represent RGB values.
uniform vec4 lightColor;
uniform vec4 BaseColor;
uniform vec4 SpecColor;
uniform float SpecPower;
uniform float RimScalar;
*/
uniform sampler2D diffuseTexture;

const float MaterialThickness = 1.0;
const vec3 ExtinctionCoefficient = vec3(1);
const vec4 lightColor = vec4(1);
const vec4 BaseColor = vec4(1,0,0,1);
const vec4 SpecColor = vec4(1);
const float SpecPower = 1.0;
const float RimScalar = 1.0;

in vec3 worldNormal, eyeVec, lightVec, vertPos, lightPos;
in vec2 uv;

out vec4 finalCol;

float halfLambert(in vec3 vect1, in vec3 vect2)
{
	float product = dot(vect1,vect2);
	return product * 0.5 + 0.5;
}

float blinnPhongSpecular(in vec3 normalVec, in vec3 lightVec, in float specPower)
{
	vec3 halfAngle = normalize(normalVec + lightVec);
	return pow(clamp(0.0,1.0,dot(normalVec,halfAngle)),specPower);
}

void main()
{
	float attenuation = 10.0 * (1.0 / distance(lightPos,vertPos));
	vec3 eVec = normalize(eyeVec);
	vec3 lVec = normalize(lightVec);
	vec3 wNorm = normalize(worldNormal);

	vec4 dotLN = vec4(halfLambert(lVec,wNorm) * attenuation);
	dotLN *= texture2D(diffuseTexture, uv);
	dotLN *= BaseColor;

	vec3 indirectLightComponent = vec3(MaterialThickness * max(0.0,dot(-wNorm,lVec)));
	indirectLightComponent += MaterialThickness * halfLambert(-eVec,lVec);
	indirectLightComponent *= attenuation;
	indirectLightComponent.r *= ExtinctionCoefficient.r;
	indirectLightComponent.g *= ExtinctionCoefficient.g;
	indirectLightComponent.b *= ExtinctionCoefficient.b;

	vec3 rim = vec3(1.0 - max(0.0,dot(wNorm,eVec)));
	rim *= rim;
	rim *= max(0.0,dot(wNorm,lVec)) * SpecColor.rgb;

	finalCol = dotLN + vec4(indirectLightComponent,1.0);
	finalCol.rgb += (rim * RimScalar * attenuation * finalCol.a);
	finalCol.rgb += vec3(blinnPhongSpecular(wNorm,lVec,SpecPower) * attenuation * SpecColor * finalCol.a * 0.05);
	finalCol.rgb *= lightColor.rgb;
	
}

