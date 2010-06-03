#version 150 core

uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 diffuse;

uniform float depth;
uniform float texsize;
uniform int linear_search_steps;
uniform int binary_search_steps;
uniform float linearAttenuation;

in vec2 texCoord;
in vec3 vertex_pos;
in vec3 light_pos;
in float vertex_dist;
in float light_dist;
in float dist_val;

uniform sampler2D reliefmap;
uniform sampler2D texmap;

out vec4 gl_FragColor;

void ray_intersect_rm (inout vec3 dp, in vec3 ds);

void main(void)
{
   vec4 t,c;
   vec3 l,s;
   vec3 pt_eye, pt_light;
   float a;

   // ray intersect in view direction
   a  = -depth / vertex_pos.z;
   s = vertex_pos * a;
   s.z = 1.0;
  
   //	find the distance to the actualy heightfield
   pt_eye = vec3 (texCoord, 0.0);
   ray_intersect_rm (pt_eye, s);
  
   /*
   gl_FragColor = vec4(pt_eye.z, 0, 0, 0);
   return;//*/
  
   // get rm and color texture points
   c=texture2D(texmap,pt_eye.xy);

   // expand normal from normal map in local polygon space
   // blue = df/dx
   // alpha = df/dy (image coords are no longer backward!)
   // note: I _need_ the texture size to scale the normal properly!
   t=texture2D(reliefmap, pt_eye.xy);
   t = vec4 ((t.ba-0.5) * (-depth * texsize), 1.0, 0.0);
   t=normalize(t);

   // adjust the hit-position
   // (only useful if the light is near relative
   // to the depth)
   //p -= v*d*a;
   
   // compute attenuation
   float att = min (1.0, 1.0 / (light_dist*linearAttenuation));
   
   // rescale the light vector
   l = att * normalize(light_pos);

   // compute the final color
   gl_FragColor = vec4(
   			ambient.xyz*c.xyz
   			+diffuse.xyz*c.xyz*max(0.0,dot(l,t.xyz)), 
   			1.0);
}

//  vanilla Relief Mapping
void ray_intersect_rm (inout vec3 dp, in vec3 ds)
{
  float depth_step=1.0/float(linear_search_steps);
  
  //  linear steps
  for (int i = 0; i < linear_search_steps; ++i)
  {
    vec4 t=texture2D(reliefmap,dp.xy);
    dp += ds * depth_step * step (dp.z, t.r);
  }
  
  //  binary search
  for (int i = 0; i < binary_search_steps; ++i)
  {
    vec4 t=texture2D(reliefmap,dp.xy);
    dp += ds * depth_step * (step (dp.z, t.r) - 0.5);
    depth_step *= 0.5;
  }  
      
  // all done
  return;
}
