#version 330 core

uniform sampler2D EarthDay;
uniform sampler2D EarthNight;
uniform sampler2D EarthCloudGloss;

in float Diffuse;
in vec3 Specular;
in vec2 uv;

out vec4 fragColor;

void main()
{
	// Monochrome cloud cover value will be in clouds.r
	// Gloss value will be in clouds.g
	// clouds.b will be unused
	vec2 clouds    = texture(EarthCloudGloss, uv).rg;
	vec3 daytime   = (texture(EarthDay, uv).rgb * Diffuse +
                         Specular * clouds.g) * (1.0 - clouds.r) +
                         clouds.r * Diffuse;
	vec3 nighttime = texture(EarthNight, uv).rgb *
                        (1.0 - clouds.r) * 2.0;
	
	if (Diffuse < 0.1){
		fragColor = vec4(mix(nighttime, daytime, (Diffuse + 0.1) * 5.0), 1.0);
	}else{
		fragColor = vec4(daytime, 1.0);
	}
}

