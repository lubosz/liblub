#version {{ version }}

uniform float MaxIterations;
uniform float Zoom;
uniform float Xcenter;
uniform float Ycenter;

uniform vec3 InnerColor;
uniform vec3 OuterColor1;
uniform vec3 OuterColor2;

in vec3 Position;
in float LightIntensity;
out vec4 FragColor;

void main()
{

	float real = Position.x * Zoom + Xcenter;
	float imag = Position.y * Zoom + Ycenter;
	float Creal = real; // Change this line...
	float Cimag = imag; // ...and this one to get a Julia set
	float r2 = 0.0;
	float iter;

	for (iter = 0.0; iter < MaxIterations && r2 < 4.0; ++iter)
	{
		float tempreal = real;
		real = (tempreal * tempreal) - (imag * imag) + Creal;
		imag = 2.0 * tempreal * imag + Cimag;
		r2 = (real * real) + (imag * imag);
	}

	// Base the color on the number of iterations
	vec3 color;
	
	if (r2 < 4.0)
		color = InnerColor;
	else
		color = mix(OuterColor1, OuterColor2, fract(iter * 0.05));
	
	color *= LightIntensity;
	FragColor = vec4(color, 1.0);

}
