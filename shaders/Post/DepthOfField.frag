{% extends "base.frag" %}

{% block linkage %}
uniform mat4 VPIMatrix;
uniform mat4 VMatrix;
uniform mat4 PMatrix;
uniform mat4 MVMatrix;

uniform sampler2D color_map;
uniform sampler2D depth_map;

uniform float pixel_width;
uniform float pixel_height;


float NearPlane = 1;
float FocusPlane = 2;
float FarPlane = 50;

float GaussMask3x3[9] = float[]
( 
	0.066666667, 0.133333333, 0.066666667,
	0.133333333, 0.200000000, 0.133333333,
	0.066666667, 0.133333333, 0.066666667
);
float GaussMask5x5[25] = float[]
( 
	0.003021148, 0.012084592, 0.021148036, 0.012084592, 0.003021148,
	0.012084592, 0.060422961, 0.099697885, 0.060422961,	0.012084592,
	0.021148036, 0.099697885, 0.166163142, 0.099697885, 0.021148036,
	0.012084592, 0.060422961, 0.099697885, 0.060422961, 0.012084592,
	0.003021148, 0.012084592, 0.021148036, 0.012084592, 0.003021148
);
//float cutoff = 40;

/*
float NearPlane = 1.f/50;
float FocusPlane = 5.f/50;
float FarPlane = 50.f/50;
*/

float tA = FarPlane / (FarPlane - NearPlane);
float tB = (-FarPlane * NearPlane) / (FarPlane - NearPlane);

vec2 even[8] = vec2[](
	vec2( 0, 1),
	vec2( 1, 0),
	vec2( 0, -1),
	vec2( -1, 0),
	vec2( 0.707, 0.707),
	vec2( 0.707, -0.707),
	vec2( -0.707, 0.707),
	vec2( -0.707, -0.707)
);

vec2 poisson[8] = vec2[](
	vec2( 0.0, 0.0),
	vec2( 0.527837,-0.085868),
	vec2(-0.040088, 0.536087),
	vec2(-0.670445,-0.179949),
	vec2(-0.419418,-0.616039),
	vec2( 0.440453,-0.639399),
	vec2(-0.757088, 0.349334),
	vec2( 0.574619, 0.685879)
	);

in vec2 uv;
{% endblock %}

{% block main %}
	fragColor = texture(color_map, uv);
	float depth = texture(depth_map, uv).r; 
	
	// compute world position (including z-value)
	vec4 vp = vec4(uv.x * 2 -1, (1 - uv.y) * 2 -1, depth, 1); // viewport position of current pixel
	
	vec4 d = VPIMatrix * vp;
	vec4 wp = d / d.w; // world position
	
	float ldepth = tB / (depth - tA); // linear depth
	
	//float z = PMatrix[3].z/(gl_FragCoord.z * -2.0 + 1.0 - PMatrix[2].z); //D / (F * -2 + 1 - C) = V
	//float z = vec4(VMatrix * wp).b;
	
	float intensity = 0;
	if(ldepth > FocusPlane) intensity = (ldepth - FocusPlane) / (FarPlane - FocusPlane); //[0..1];
	//else intensity = 10 * (FocusPlane - ldepth) / (FocusPlane - NearPlane); //[0..1];
	float radius = intensity * 3;
	vec4 sum = vec4(0);
	
	if(intensity > 0.1)
	{
		// Gauss
		fragColor = vec4(0);
		for(int x = 0; x < 3; x++)
		{
			for(int y = 0; y < 3; y++)
			{
				vec2 coord = uv;
				coord.x += (x - 1) * pixel_width * radius;
				coord.y += (y - 1) * pixel_height * radius;
				fragColor += texture(color_map, coord) * GaussMask3x3[y*3+x];
			}
		}
		//fragColor = mix(fragColor, sum, intensity);
		
		// poisson
		for(int i = 0; i < 8; i++)
		{
			vec2 offset = poisson[i];
			offset.x *= pixel_width;
			offset.y *= pixel_height;
			vec2 coord = uv + offset * intensity * 6;
	
			vec4 fetch = texture(color_map, coord);
			fragColor = mix(fragColor, fetch, 0.125);
		}
	}	
{% endblock %}
