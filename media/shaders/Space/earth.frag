#version 150 core

uniform sampler2D EarthDay;
uniform sampler2D EarthNight;
uniform sampler2D EarthCloudGloss;

in float Diffuse;
in vec3 Specular;
in vec2 TexCoord;

out vec4 FragColor;

void main()
{
    // Monochrome cloud cover value will be in clouds.r
    // Gloss value will be in clouds.g
    // clouds.b will be unused
  vec2 clouds    = texture(EarthCloudGloss, TexCoord).rg;
  vec3 daytime   = (texture(EarthDay, TexCoord).rgb * Diffuse +
                         Specular * clouds.g) * (1.0 - clouds.r) +
                         clouds.r * Diffuse;
  vec3 nighttime = texture(EarthNight, TexCoord).rgb *
                        (1.0 - clouds.r) * 2.0;
  vec3 color = daytime;
  if (Diffuse < 0.1)
      color = mix(nighttime, daytime, (Diffuse + 0.1) * 5.0);
  FragColor = vec4(color, 1.0);
}

