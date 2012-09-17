#version {{ version }}

//precision highp float;

in vec3 in_Vertex;
//in vec3 in_Normal;
//in vec3 in_Tangent;
//in vec3 in_Bitangent;
in vec2 in_Uv;

//out vec2 uv;
/*
out vec3 viewNormal;
out float viewDistance;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
*/

out vec4 viewPosition;
out vec4 thePosition;
out vec2 texCoord;
out vec4 lightPositionView;

uniform mat4 MVPMatrix;

void main(void) {
    //uv = in_Uv;
    //viewNormal = NormalMatrix * in_Normal;


    //viewDistance = length(MVMatrix *movedVertex);
    
     // float4x4 ModelView = glstate.matrix.modelview[0];
  //float4x4 ModelViewProj = glstate.matrix.mvp;
 
  // Transform vertex
  //OUT.Position = mul( ModelViewProj, IN.Position );
  //OUT.Pos =  mul( ModelViewProj, IN.Position ); 
  //OUT.TexCoord = IN.TexCoord;
  //OUT.Color = IN.Color;
  texCoord = in_Uv;
  thePosition = vec4(in_Vertex,1);
  lightPositionView = MVPMatrix * vec4(2,1,0.5,1);
  viewPosition = MVPMatrix * vec4(in_Vertex,1);
    gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
