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

const float SpecularContribution = 0.3;
const float DiffuseContribution = 1.0 - SpecularContribution;

out float    LightIntensity;

void main()
{   

    vec4 MCvertex = vec4(in_Vertex,1);
    //vec3 ecPosition =  vec3(MVMatrix * MCvertex);
    vec3 ecPosition =  vec3(MVMatrix * MCvertex);
    vec3 tnorm      =  normalize(NormalMatrix * in_Normal);
    //vec3 lightVec   =  normalize(LightPosition - ecPosition);
	//vec3 lightVec   =  normalize(LightPosition);
vec3 lightVec = normalize(-LightPosition.xzy);
   lightVec.y = -lightVec.y;
    vec3 reflectVec =  reflect(-lightVec, tnorm);
    vec3 viewVec    =  normalize(-ecPosition);
    float diffuse   =  max(dot(lightVec, tnorm), 0.0);
    float spec      =  0.0;
    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }
    LightIntensity  = DiffuseContribution * diffuse +
                       SpecularContribution * spec;


   //Pass the texture coordinate on through.
   //gl_TexCoord[0] = gl_MultiTexCoord0;
   TexCoord = in_Uv;

/*
   vLightDir = normalize(-LightPosition.xzy);
   vLightDir.y = -vLightDir.y;
  */
vLightDir = lightVec;
 
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


