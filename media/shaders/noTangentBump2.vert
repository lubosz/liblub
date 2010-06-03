#version 150 core

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 TexCoord;

out vec3 lightDir; 
out vec3 viewDir;
out vec3 normal;

void main(void)
{
	//gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	TexCoord = in_Uv;

	normal = normalize(NormalMatrix * in_Normal);
	
	//note: this is a directional light!
	//lightDir = normalize(gl_LightSource[0].position.xyz);
	lightDir = normalize(LightPosition);
	
	vec3 eyeVec = vec3(MVMatrix * vec4(in_Vertex,1.0));
	viewDir = normalize(-eyeVec);
	
	//gl_Position = ftransform();
	gl_Position     = MVPMatrix * vec4(in_Vertex,1);
}
