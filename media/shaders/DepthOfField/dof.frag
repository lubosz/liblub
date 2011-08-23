#version 330 core

uniform sampler2D color_map;
uniform sampler2D depth_map;

uniform float pixel_width;
uniform float pixel_height;

float NearPlane = 1;
float FocusPlane = 10;
float FarPlane = 100;

float MaxCOCRadius;


/*
*/
vec2 maxCoC = vec2(5.0,10.0);
float radiusScale = 0.5;


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

out vec4 fragColor;

void main()
{
/*
	vec4 finalColor;

	float discRadius, discRadiusLow, centerDepth;

	finalColor = texture(color_map, uv);
	centerDepth = texture(depth_map, uv);

	discRadius = abs(centerDepth * maxCoC.y - maxCoC.x);
	discRadiusLow = discRadius * radiusScale;

	finalColor = vec4(0.0,0.0,0.0,0.0);

	for(int i = 0; i < 8; i++){

		//vec2 coordLow = coords + (pixelSizeLow * poisson[i] * discRadiusLow);
		vec2 coordHigh = coords + (pixel_height * poisson[i] * discRadius);

		//vec4 tapLow = texture2D(textureSmall, coordLow);
		vec4 tapHigh = texture(color_map, coordHigh);
		

		float tapBlur = abs(texture(depth_map, coordHigh) * 2.0 - 1.0);
		vec4 tap = mix(tapHigh,tapLow,tapBlur);

		//tap.a = (tap.a >= centerDepth) ? 1.0 : abs(tap.a * 2.0 - 1.0);

		finalColor.rgb += tap.rgb * tap.a;
		//finalColor.a += tap.a;

	}

	return finalColor/finalColor.a;
*/

	fragColor = texture(color_map, uv);
	float depth = texture(depth_map, uv).r;
	
	// circle of confusion (radius)
	
	//float coc_r  = abs(FocusPlane * FarPlane - NearPlane);
	float coc_r = abs(depth - NearPlane);//1000*depth;// * g_MaxCOCRadius / 4;

	// circle of confusion (diameter)
	//float coc_d = 
	
	
	
	fragColor = vec4(depth);
	
	/*	
	for(int i = 0; i < 8; i++)
	{
		vec2 offset = poisson[i];
		offset.x *= pixel_width;
		offset.y *= pixel_height;

		vec4 fetch = texture(color_map, uv + offset * coc_r);
		fragColor = mix(fragColor, fetch, 0.125);
	}
	*/
}

/*
vec2 maxCoC = vec2(5.0,10.0);
float radiusScale = 0.5;


vec2 pixelSizeHigh, pixelSizeLow;
float depth;

vec4 dof(vec2 coords){



}

void main(){

	pixelSizeHigh[0] = 1.0/float(windowwidth);
	pixelSizeHigh[1] = 1.0/float(windowheight);

	pixelSizeLow[0] = 1.0/float(windowwidth/2);
	pixelSizeLow[1] = 1.0/float(windowheight/2);

	//this won't work etiher on older graphics cards
	// but I don't know how to fix it
	// poisson-distributed positions
	poisson = vec2[8](
	vec2( 0.0, 0.0),
	vec2( 0.527837,-0.085868),
	vec2(-0.040088, 0.536087),
	vec2(-0.670445,-0.179949),
	vec2(-0.419418,-0.616039),
	vec2( 0.440453,-0.639399),
	vec2(-0.757088, 0.349334),
	vec2( 0.574619, 0.685879)
	);

	gl_FragColor = dof(gl_TexCoord[0].xy);

}
*/