#version 150 compatibility

/*
in vec4  gl_Vertex;
in vec3  gl_Normal;
in vec4  gl_MultiTexCoord0;
in vec4  gl_MultiTexCoord1;
in vec4  gl_MultiTexCoord2;
*/

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
   eyeSpaceVert = (gl_ModelViewMatrix * gl_Vertex).xyz;
   
   // convert to eyeSpace for programs other than Irrlicht
   //eyeSpaceLight = (gl_ModelViewMatrix * vec4(gl_LightSource[0].position.xyz,1.0)).xyz;
   
   // gl_LightSource[n].position is already in eyeSpace from Irrlicht!
   eyeSpaceLight = gl_LightSource[0].position.xyz;
   
  //*
   eyeSpaceTangent  = gl_NormalMatrix * gl_MultiTexCoord1.xyz;	//tangent;
   eyeSpaceBinormal = gl_NormalMatrix * (-gl_MultiTexCoord2.xyz);	//binormal;
   eyeSpaceNormal   = gl_NormalMatrix * gl_Normal;
  //*/
  
   /*
   eyeTransform = mat3 (
      gl_NormalMatrix * (gl_MultiTexCoord1.xyz),
      gl_NormalMatrix * (-gl_MultiTexCoord2.xyz),
      gl_NormalMatrix * (gl_Normal.xyz)
   );
   //*/
   
   texCoord   = gl_MultiTexCoord0.xy;
   
   gl_Position = ftransform();
}
