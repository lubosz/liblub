//VERTEX SHADER
#version 150
//UNIFORM
//uniform mat4 ProjectionModelviewMatrix;
//uniform vec4 TexMatrix0_a;
//uniform vec4 TexMatrix0_b;
//uniform vec4 LightPosition0;
//uniform mat4 ModelviewMatrix;
//VARYING
//varying vec2 VTexCoord0;
out vec3 VHalfVector0;
out vec3 VEyeNormal;
out vec3 VEyeVertex;
//GLOBALFUNCTION
void main(){
	vec3 eyeVertex;
	vec3 lightVector, eyeVector;
	//gl_Position = ProjectionModelviewMatrix * gl_Vertex;
	gl_Position = ftransform();
	//VTexCoord0.x = dot(TexMatrix0_a, gl_MultiTexCoord0);
	//VTexCoord0.y = dot(TexMatrix0_b, gl_MultiTexCoord0);
	//eyeVertex = vec3(ModelviewMatrix * gl_Vertex);
	eyeVertex = vec3(gl_ModelViewMatrix * gl_Vertex);
	VEyeVertex = eyeVertex;
	eyeVector = normalize(-eyeVertex);
	//lightVector = LightPosition0.xyz;
	lightVector = gl_LightSource[0].position.xyz;
	VHalfVector0 = lightVector + eyeVector;					//No need to normalize the sum
	//VEyeNormal = vec3(ModelviewMatrix * vec4(gl_Normal, 0.0));
	VEyeNormal = vec3(gl_ModelViewMatrix * vec4(gl_Normal, 0.0));
}

