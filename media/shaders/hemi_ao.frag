#version 150 core

uniform vec3 BaseColor;
uniform float SpecularPercent;
uniform float DiffusePercent;
uniform samplerCube SpecularEnvMap;
uniform samplerCube DiffuseEnvMap;

in  vec3 ReflectDir;
in  vec3 Normal;
in  float Accessibility;

out vec4  FragColor;

void main()
{
  // Look up environment map values in cube maps
  vec3 diffuseColor =
      vec3(texture(DiffuseEnvMap,  normalize(Normal)));
  vec3 specularColor =
      vec3(texture(SpecularEnvMap, normalize(ReflectDir)));
  // Add lighting to base color and mix
  vec3 color = mix(BaseColor, diffuseColor*BaseColor, DiffusePercent);
  color     *= Accessibility;
  color      = mix(color, specularColor + color, SpecularPercent);
  FragColor = vec4(color, 1.0);
}

