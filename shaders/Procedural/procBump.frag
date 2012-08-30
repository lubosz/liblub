#version 330 core

uniform vec4 SurfaceColor;
uniform float BumpDensity;
uniform float BumpSize;
uniform float SpecularFactor;

in vec3 LightDir;
in vec3 EyeDir;
in vec2 uv;

out vec4 fragColor;

void main()
{
	vec2 c = BumpDensity * uv;
	vec2 p = fract(c) - vec2(0.5);
	float d, f;
	d = dot(p,p);
	f = inversesqrt(d + 1.0);
	if (d >= BumpSize)
		{ p = vec2(0.0); f = 1.0; }
	vec3 normDelta = vec3(p.x, p.y, 1.0) * f;
	vec3 litColor = SurfaceColor.rgb * max(dot(normDelta, LightDir), 0.0);
	vec3 reflectDir = reflect(LightDir, normDelta);
	float spec = max(dot(EyeDir, reflectDir), 0.0);
	spec = pow(spec, 6.0);
	spec *= SpecularFactor;
	litColor = min(litColor + spec, vec3(1.0));
  
	fragColor = vec4(litColor, SurfaceColor.a);
}

