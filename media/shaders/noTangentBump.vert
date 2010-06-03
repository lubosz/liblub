#version 150 core

//uniform mat4 osg_ViewMatrixInverse;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 TexCoord;

out vec3 vNormal;
out vec3 vLightDir;
out vec3 vViewDir;

void main()
{   
   //Pass the texture coordinate on through.
   //gl_TexCoord[0] = gl_MultiTexCoord0;
   TexCoord = in_Uv;

   vLightDir = normalize(-LightPosition.xzy);
   vLightDir.y = -vLightDir.y;
   
   // View position  -  vertex position
   //vViewDir = normalize(osg_ViewMatrixInverse[3].xyz - in_Vertex);
   	vec3 eyeVec = vec3(MVMatrix * vec4(in_Vertex,1.0));
	vViewDir = normalize(-eyeVec);

   
   // Get the world space normal
   /*
   mat4 worldMatrix = MVMatrix * osg_ViewMatrixInverse;
   vec3 test = (NormalMatrix * in_Normal);
   test = osg_ViewMatrixInverse * vec4(test, 0.0);
   vNormal = normalize(test);
  */
   vNormal = normalize(NormalMatrix * in_Normal);
   
   //Compute the final vertex position in clip space.
   gl_Position     = MVPMatrix * vec4(in_Vertex,1);
}


