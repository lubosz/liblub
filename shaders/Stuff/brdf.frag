#version 420 core

const float PI = 3.14159;
const float ONE_OVER_PI = 1.0 / PI;

/*
uniform vec4 SurfaceColor; // Base color of surface
uniform vec2 P; // Diffuse (x) and specular reflectance (y)
uniform vec2 A; // Slope distribution in x and y
uniform vec3 Scale; // Scale factors for intensity computation
*/


const vec4 SurfaceColor = vec4(1,0,0,1); // Base color of surface
const vec2 P = vec2(0.8, 0.7); // Diffuse (x) and specular reflectance (y)
const vec2 A = vec2(1,1); // Slope distribution in x and y
const vec3 Scale = vec3(1,1,1); // Scale factors for intensity computation


in  vec3  N, L, H, R, T, B;

out vec4  FragColor;

void main()
{
   float e1, e2, E, cosThetaI, cosThetaR, brdf, intensity;
   e1 = dot(H, T) / A.x;
   e2 = dot(H, B) / A.y;
   E = -2.0 * ((e1 * e1 + e2 * e2) / (1.0 + dot(H, N)));
   cosThetaI = dot(N, L);
   cosThetaR = dot(N, R);
   brdf = P.x * ONE_OVER_PI +
          P.y * (1.0 / sqrt(cosThetaI * cosThetaR)) *
          (1.0 / (4.0 * PI * A.x * A.y)) * exp(E);
  intensity = Scale[0] * P.x * ONE_OVER_PI +
              Scale[1] * P.y * cosThetaI * brdf +
              Scale[2] * dot(H, N) * P.y;
  vec3 color = intensity * SurfaceColor.rgb;
  FragColor = vec4(color, 1.0);
  
  FragColor = vec4(brdf);
}

