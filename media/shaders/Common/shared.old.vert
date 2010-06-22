#version 150 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

//  starting texture coordinate
out vec2 texCoord;
//  position and light in eye space
out vec3 eyeSpaceVert;
out vec3 eyeSpaceLight;
//  the matrix needed to convert to eye space
//varying mat3 eyeTransform;
//*
out vec3 eyeSpaceTangent;
out vec3 eyeSpaceBinormal;
out vec3 eyeSpaceNormal;
//*/


void main(void)
{ 
   eyeSpaceVert = (MVMatrix * vec4(in_Vertex,1)).xyz;
   
   // convert to eyeSpace for programs other than Irrlicht
   eyeSpaceLight = (MVMatrix * vec4(LightPosition,1.0)).xyz;
   
   // gl_LightSource[n].position is already in eyeSpace from Irrlicht!
   //eyeSpaceLight = LightPosition;
   
  //*
   eyeSpaceTangent  = NormalMatrix * vec3(in_Uv,1);	//tangent;
   eyeSpaceBinormal = NormalMatrix * (-vec3(in_Uv,1));	//binormal;
   eyeSpaceNormal   = NormalMatrix * in_Normal;
  //*/
  
   /*
   eyeTransform = mat3 (
      gl_NormalMatrix * (gl_MultiTexCoord1.xyz),
      gl_NormalMatrix * (-gl_MultiTexCoord2.xyz),
      gl_NormalMatrix * (gl_Normal.xyz)
   );
   //*/
   
   texCoord   = in_Uv;
   
   gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
