#version 150 core

uniform vec4 ambient;
uniform vec4 diffuse;
uniform vec3 LightPosition;

uniform float depth;
uniform float texsize;
uniform float csm_gain;
uniform float csm_offset;
uniform float linearAttenuation;
uniform int conesteps;

uniform sampler2D stepmap;
uniform sampler2D texmap;
   
in vec2 texCoord;
in vec3 eyeSpaceVert;
//in vec3 LightPosition;

in vec3 eyeSpaceTangent;
in vec3 eyeSpaceBinormal;
in vec3 eyeSpaceNormal;

out vec4 FragColor;

void intersect_cone_exp (inout vec3 dp, in vec3 ds, in float dist_factor);
void intersect_cone_fixed(inout vec3 dp, in vec3 ds);

void main(void)
{
   vec4 t,c;
   vec3 p,v,l,s;
   vec3 pt_eye, pt_light;
   float a,df;

   // ray intersect in view direction
   p  = eyeSpaceVert;
   v  = normalize(p);
   a  = -depth / dot(eyeSpaceNormal,v);
   s = vec3(dot(v,eyeSpaceTangent)*a,dot(v,eyeSpaceBinormal)*a,1.0);
   
   //	find the distance to the actualy heightfield
   df = 0.00333 * length (p) * csm_gain + csm_offset;
   df /= texsize;
  
   //	find the distance to the actualy heightfield
   pt_eye = vec3 (texCoord, 0.0);
   intersect_cone_exp (pt_eye, s, df);
  
   // get rm and color texture points
   c=texture2D(texmap,pt_eye.xy);

   // expand normal from normal map in local polygon space
   // blue = df/dx
   // alpha = df/dy (image coords are no longer backward!)
   // note: I _need_ the texture size to scale the normal properly!
   t=texture2D(stepmap, pt_eye.xy);
   t = vec4 ((t.ba-0.5) * (-depth * texsize), 1.0, 0.0);
   t.xyz=normalize(t.x*eyeSpaceTangent+t.y*eyeSpaceBinormal+t.z*eyeSpaceNormal);

   // adjust the hit-position
   // (only useful if the light is near relative
   // to the depth)
   //p -= v*pt_eye.z;
   
   // compute light direction
   l=LightPosition.xyz-p;
   
   // compute attenuation
   float att = linearAttenuation;
   att = min (1.0, inversesqrt(dot(l,l)*att*att));
   
   //	now the self-shadowing
   a  = -depth / dot(eyeSpaceNormal,l);
   s = vec3(dot(l,eyeSpaceTangent)*a,dot(l,eyeSpaceBinormal)*a,1.0);

   // move the texture start position to where the light-ray
   // would intersect the surface of this polygon.
   pt_light = vec3 ( pt_eye.xy - s.xy * pt_eye.z / s.z, 0.0);
   //uv -= s.xy*(d*a/s.z);
   
   //	with the soft shadows, _fixed is fine
   intersect_cone_fixed(pt_light,s);
   
   // do the softening and ignore if it's facing the wrong way
   float shad_z = clamp (1.0+(-pt_eye.z+pt_light.z)*5.0, 0.0, 1.0) * step (a, 0.0);
   
   // add back in attenuation
   // rescale the light vector
   // (shadow^2 make it more in shadow, but keeps the soft edge!)
   l = normalize(l) * att * shad_z*shad_z;

   // compute the final color
   FragColor = vec4(
   			ambient.xyz*c.xyz+
   			c.xyz*diffuse.xyz*max(0.0,dot(l,t.xyz)), 
   			1.0);
}

// experimental (distance is taken into account)
void intersect_cone_exp(inout vec3 dp, in vec3 ds, in float dist_factor)
{
   // the "not Z" component of the direction vector
   float iz = length(ds.xy);
   // my starting location (is at z=0)
   // texture lookup
   vec4 t;
   // and how high above the surface am I?
   float ht, old_ht;
   float CR = 0.0;
  
   //	find the starting location and height
   t=texture2D(stepmap,dp.xy);
   while (t.r > dp.z)
   {
     CR = t.g * t.g;

	   // right, I need to take one step.
	   // I use the current height above the texture,
	   // and the information about the cone-ratio
	   // to size a single step.  So it is fast and 
	   // precise!  (like a coneified version of
	   // "space leaping", but adapted from voxels)
     
     // experimental!!!
     // and take the step
     dp += ds * (dist_factor + (t.r - dp.z)*CR) / (iz + CR);
     
	   //	find the new location and height
	   t=texture2D(stepmap,dp.xy);
   }
   
   // back out to where the cone was (remove the w component)
   //*
   ht = (t.r - dp.z);
   dist_factor /= (iz + CR);
   dp -= ds * dist_factor;
   //*/
   // and sample it
   //*
   t=texture2D(stepmap,dp.xy);
   old_ht = t.r - dp.z;
   // linear interpolation
   dp += ds * dist_factor * (1.0 - clamp (ht / (ht - old_ht), 0.0, 1.0));
   //*/
   
   
   // and a nice cone step
   //*
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   //*/
   
   // all done
   return;
}

//	the super fast version 
// (change number of iterations at compile time)
void intersect_cone_fixed(inout vec3 dp, in vec3 ds)
{
   // the "not Z" component of the direction vector
   // (requires that the vector ds was normalized!)
   float iz = length(ds.xy);
   // my starting location (is at z=1, 
   // and moving down, so I inverted 
   // theheight maps)
   // texture lookup (and initialized to starting location)
   vec4 t;

   //	find the initial location and height
   t=texture2D(stepmap,dp.xy);
   // right, I need to take one step.
   // I use the current height above the texture,
   // and the information about the cone-ratio
   // to size a single step.  So it is fast and 
   // precise!  (like a coneified version of
   // "space leaping", but adapted from voxels)
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   
   // and repeat a few (4x) times 
   //*
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   
   /*
   // and another five!
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   */
   
   /*
   // and another five!
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r + ds.z*sc) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r + ds.z*sc) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r + ds.z*sc) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r + ds.z*sc) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * (t.r + ds.z*sc) / (iz/(t.g*t.g) + 1.0);
   //*/
   
   // all done
   return;
}
