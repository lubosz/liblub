#version 150 core

uniform vec4  SurfaceColor;    //  = (0.7, 0.6, 0.18, 1.0)
uniform float BumpDensity;     //  = 16.0
uniform float BumpSize;        //  = 0.15
uniform float SpecularFactor;  //  = 0.5

in  vec3 LightDir;
in  vec3 EyeDir;
in  vec2 TexCoord;

out vec4 FragColor;

void main()
{
    vec3 litColor;
    vec2 c = BumpDensity * TexCoord.st;
    vec2 p = fract(c) - vec2(0.5);
    float d, f;
    d = dot(p,p);
    f = inversesqrt(d + 1.0);
    if (d >= BumpSize)
        { p = vec2(0.0); f = 1.0; }
  vec3 normDelta = vec3(p.x, p.y, 1.0) * f;
  litColor = SurfaceColor.rgb * max(dot(normDelta, LightDir), 0.0);
  vec3 reflectDir = reflect(LightDir, normDelta);
  float spec = max(dot(EyeDir, reflectDir), 0.0);
  spec = pow(spec, 6.0);
  spec *= SpecularFactor;
  litColor = min(litColor + spec, vec3(1.0));
  
  FragColor = vec4(litColor, SurfaceColor.a);
}

