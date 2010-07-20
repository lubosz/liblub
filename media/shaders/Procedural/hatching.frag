#version 330

uniform sampler3D Noise; // value of Noise = 3;
const float frequency = 1.0;

in vec3 ObjPos; // object space coord (noisy)
in float V; // generic varying
in float LightIntensity;
out vec4 FragColor;

void main()
{
	float dp = length(vec2(dFdx(V), dFdy(V)));
	float logdp = -log2(dp * 8.0);
	float ilogdp = floor(logdp);
	float stripes = exp2(ilogdp);
	float noise = texture(Noise, ObjPos).x;
	float sawtooth = fract((V + noise * 0.1) * frequency * stripes);
	float triangle = abs(2.0 * sawtooth - 1.0);
	// adjust line width
	float transition = logdp - ilogdp;
	// taper ends
	triangle = abs((1.0 + transition) * triangle - transition);
	const float edgew = 0.3; // width of smooth step
	float edge0 = clamp(LightIntensity - edgew, 0.0, 1.0);
	float edge1 = clamp(LightIntensity, 0.0, 1.0);
	float square = 1.0 - smoothstep(edge0, edge1, triangle);
	FragColor = vec4(vec3(square), 1.0);
}
