#version 150 core

//  input from the user
uniform float csm_gain;
uniform float csm_offset;
uniform float texsize;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Binormal;
in vec3 in_Tangent;
in vec2 in_Uv;

//  interpolate these and pass 
//  them into the fragment shader
out vec2 texCoord;
out vec3 vertex_pos;
out vec3 light_pos;
out float vertex_dist;
out float light_dist;

//  distance to the point
out float dist_val;

void main(void)
{
   // location of the vertex in eye space
   vec3 eyeSpaceVert = (MVMatrix * vec4(in_Vertex,1)).xyz;
  
   // convert to eyeSpace for programs other than Irrlicht
   //eyeSpaceLight = (gl_ModelViewMatrix * vec4(gl_LightSource[0].position.xyz,1.0)).xyz;
   // gl_LightSource[n].position is already in eyeSpace from Irrlicht!
   vec3 eyeSpaceLight = LightPosition.xyz;
   

	vec3 eyeSpaceNormal = normalize(NormalMatrix * in_Normal);
	mat3 trans = transpose(NormalMatrix);
    vec3 eyeSpaceTangent = normalize(NormalMatrix * trans[0]); 
	vec3 eyeSpaceBinormal = normalize(NormalMatrix * trans[1]);


   //  the matrix needed to convert to eye space
   //  (this is local, and should already be normalized, I think)
/*
   vec3 eyeSpaceTangent  = normalize(NormalMatrix * in_Tangent);	//tangent;
   vec3 eyeSpaceBinormal = normalize(NormalMatrix * in_Binormal);	//binormal;
   vec3 eyeSpaceNormal   = normalize(NormalMatrix * in_Normal); //normal
  */
   // now convert the light and position, and pass in the texture coordinate
   vertex_pos = vec3 (
        dot (eyeSpaceTangent, eyeSpaceVert),
        dot (eyeSpaceBinormal, eyeSpaceVert),
        dot (eyeSpaceNormal, eyeSpaceVert));
   light_pos = vec3 (
        dot (eyeSpaceTangent, eyeSpaceLight),
        dot (eyeSpaceBinormal, eyeSpaceLight),
        dot (eyeSpaceNormal, eyeSpaceLight))
        - vertex_pos;
   vertex_dist = length (eyeSpaceVert);
   light_dist = length (light_pos);
   //light_pos /= light_dist;
   texCoord   = in_Uv;
   
   //  for the LOD
   dist_val = (0.00333 * vertex_dist * csm_gain + csm_offset) 
      / texsize;
   
   // done
   gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
