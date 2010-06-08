varying vec3 blend;
varying vec2 TexCoords0;
varying vec2 TexCoords1;
varying vec2 TexCoords2;

uniform float depth;
uniform float texsize;

varying vec3 vertex_pos0;
varying vec3 vertex_pos1;
varying vec3 vertex_pos2;
varying vec3 light_pos0;
varying vec3 light_pos1;
varying vec3 light_pos2;
varying float vertex_dist;
varying float light_dist0;
varying float light_dist1;
varying float light_dist2;
varying float dist_val;

uniform sampler2D stepmap;
uniform sampler2D texmap;

void intersect_square_cone_exp (inout vec3 dp, in vec3 ds, in float dist_factor);
void intersect_square_cone_10step (inout vec3 dp, in vec3 ds);

void main(void)
{

	vec3 vertex_pos = vertex_pos0 * blend.x + vertex_pos1 * blend.y + vertex_pos2 * blend.z;
	vec3 light_pos = light_pos0 * blend.x + light_pos1 * blend.y + light_pos2 * blend.z;
	float light_dist = light_dist0 * blend.x + light_dist1 * blend.y + light_dist2 * blend.z;
	vec2 texCoord = TexCoords0 * blend.x + TexCoords1 * blend.y + TexCoords2 * blend.z;

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
   intersect_square_cone_exp (pt_eye, s, dist_val);
  
   // get rm and color texture points
   c=texture2D(texmap,pt_eye.xy);

   // expand normal from normal map in local polygon space
   // blue = df/dx
   // alpha = df/dy (image coords are no longer backward!)
   // note: I _need_ the texture size to scale the normal properly!
   t=texture2D(stepmap, pt_eye.xy);
   t = vec4 ((t.ba-0.5) * (-depth * texsize), 1.0, 0.0);
   t=normalize(t);

   // adjust the hit-position
   // (only useful if the light is near relative
   // to the depth)
   //p -= v*d*a;
   
   // constant attenuation
   float att = 1.0;
   
   // compute the self shadowing
   a  = -depth / light_pos.z;
   s = light_pos * a;
   s.z = 1.0;


   // move the texture start position to where the light-ray
   // would intersect the surface of this polygon.
   pt_light = vec3 ( pt_eye.xy - s.xy * pt_eye.z / s.z, 0.0);
   
   //	with the soft shadows, _fixed is fine
   intersect_square_cone_10step (pt_light,s);
   
   // do the softening and ignore if it's facing the wrong way
   float shad_z = clamp (1.0+(-pt_eye.z+pt_light.z)*5.0, 0.0, 1.0) * step (a, 0.0);
   
   // add back in attenuation
   // rescale the light vector
   // (shadow^2 make it more in shadow, but keeps the soft edge!)
   l = normalize(light_pos) * att * shad_z*shad_z;

	//diffuse
   	gl_FragColor = vec4(c.xyz*max(0.0,dot(l,t.xyz)), 1.0);
}

// experimental (distance is taken into account)
void intersect_square_cone_exp (inout vec3 dp, in vec3 ds, in float dist_factor)
{
   // the "not Z" component of the direction vector (for a square cone)
   float iz = max(abs(ds.x),abs(ds.y));
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

// just take 10 steps!
void intersect_square_cone_10step (inout vec3 dp, in vec3 ds)
{
   // the "not Z" component of the direction vector (for a square cone)
   float iz = max(abs(ds.x),abs(ds.y));
   // SOR
   const float w = 1.0;
   // my starting location (is at z=0)
   // texture lookup
   vec4 t;

   //	find the initial location and height
   t=texture2D(stepmap,dp.xy);
   // right, I need to take one step.
   // I use the current height above the texture,
   // and the information about the cone-ratio
   // to size a single step.  So it is fast and 
   // precise!  (like a coneified version of
   // "space leaping", but adapted from voxels)
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   
   // and repeat a few (4) times 
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   
   // and another 5 times 
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
   t=texture2D(stepmap,dp.xy);
   dp += ds * w * (t.r - dp.z) / (iz/(t.g*t.g) + 1.0);
      
   // all done
   return;
}
