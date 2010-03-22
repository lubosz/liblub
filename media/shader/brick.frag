#version 140
uniform vec3 BrickColor, MortarColor;
uniform vec2 BrickSize;
uniform vec2 BrickPct;
in vec2      MCposition;
in float     LightIntensity;
out vec4     FragColor;
void main()
{
    vec3 color;
    vec2 position, useBrick;
   position = MCposition / BrickSize;
   if (fract(position.y * 0.5) > 0.5)
       position.x += 0.5;
   position = fract(position);
   useBrick = step(position, BrickPct);
//
   color = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
   color *= LightIntensity;
   FragColor = vec4(color, 1.0);
}

