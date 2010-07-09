#version 330 core

uniform vec3 BaseColor;
uniform float MixRatio;
uniform samplerCube EnvMap;
//uniform sampler2D Map;

in vec3 ReflectDir;
in float LightIntensity;

out vec4 FragColor;

void main()
{
	// Look up environment map value in cube map
	//vec3 envColor = vec3(texture(Map, vec2(ReflectDir.x,ReflectDir.y) ));
	vec3 envColor = vec3(texture(EnvMap, ReflectDir));
	// Add lighting to base color and mix
	vec3 base = LightIntensity * BaseColor;
	//envColor = mix(envColor, base, MixRatio);
	FragColor = vec4(envColor, 1.0);
	//FragColor = vec4(ReflectDir, 1.0);
}

