#version 150 core

uniform vec4 ambient;
uniform vec4 specular;
uniform vec4 diffuse;

uniform float shine;
uniform float depth;
uniform float tile;
uniform float texsize;
uniform float linearAttenuation;

uniform int linear_search_steps;
uniform int binary_search_steps;

uniform sampler2D reliefmap;
uniform sampler2D texmap;
   
in vec2 texCoord;
in vec3 eyeSpaceVert;
in vec3 eyeSpaceTangent;
in vec3 eyeSpaceBinormal;
in vec3 eyeSpaceNormal;
in vec3 eyeSpaceLight;

out vec4 gl_FragColor;

float ray_intersect_rm(in vec2 dp,in vec2 ds);

void main(void)
{
   vec4 t,c;
   vec3 p,v,l,s;
   vec2 dp,ds,uv;
   float d,a;

   // ray intersect in view direction
   p  = eyeSpaceVert;
   v  = normalize(p);
   a  = dot(eyeSpaceNormal,-v);
   s  = normalize(vec3(dot(v,eyeSpaceTangent),dot(v,eyeSpaceBinormal),a));
   s  *= depth/a;
   dp = texCoord*tile;
   ds = s.xy;
   d  = ray_intersect_rm(dp,ds);
   
   // get rm and color texture points
   uv=dp+ds*d;
   ////uv = texCoord;
   t=texture2D(reliefmap,uv);
   c=texture2D(texmap,uv);

   // expand normal from normal map in local polygon space
   t = vec4 ((t.ba-0.5) * (-depth * texsize), 1.0, 0.0);
   t.xyz=normalize(t.x*eyeSpaceTangent+t.y*eyeSpaceBinormal+t.z*eyeSpaceNormal);

   // adjust the hit-position
   // (only useful if the light is near relative
   // to the depth)
   //p -= v*d*a;
   
   // compute light direction
   l=eyeSpaceLight.xyz-p;
   
   // compute attenuation
   float att = linearAttenuation;
   att = min (1.0, inversesqrt(dot(l,l)*att*att));
   
   // rescale the light vector
   l = att * normalize(l);

   // compute the final color
   gl_FragColor = vec4(
   			ambient.xyz*c.xyz+
   			c.xyz*diffuse.xyz*max(0.0,dot(l,t.xyz)), 
   			1.0);   
}

float ray_intersect_rm(in vec2 dp, in vec2 ds)
{
   //const int linear_search_steps=20;
   //const int binary_search_steps=5;
   float depth_step=1.0/float(linear_search_steps);

   // current size of search window
   float size=depth_step;
   // current depth position
   float depth=0.0;
   // best match found (starts with last position 1.0)
   float best_depth=1.0;

   // search front to back for first point inside object
   for( int i=0;i<linear_search_steps-1;i++ )
   {
      depth+=size;
      vec4 t=texture2D(reliefmap,dp+ds*depth);

      if (best_depth>0.996)   // if no depth found yet
      if (depth>=(t.r))
         best_depth=depth;   // store best depth
   }
   depth=best_depth;
   
   // recurse around first point (depth) for closest match
   for( int i=0;i<binary_search_steps;i++ )
   {
      size*=0.5;
      vec4 t=texture2D(reliefmap,dp+ds*depth);
      if (depth>=(t.r))
      {
         best_depth=depth;
         depth-=2.0*size;
      }
      depth+=size;
   }

   return best_depth;
}
