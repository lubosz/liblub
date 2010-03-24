#version 150 core

uniform vec3 BaseColor;
uniform float MixRatio;
uniform samplerCube EnvMap;

in vec3 ReflectDir;
in float LightIntensity;

out vec4 FragColor;

void main()
{
	// Look up environment map value in cube map
	vec3 envColor = vec3(texture(EnvMap, ReflectDir));
	// Add lighting to base color and mix
	vec3 base = LightIntensity * BaseColor;
	envColor = mix(envColor, base, MixRatio);
	FragColor = vec4(envColor, 1.0);
}

