//uniform sampler2D tex;
uniform float Time;
varying vec4 position; 
varying vec3 normal; 
void main() 
{ 

	vec3 normal = normalize(normal); 
	vec3 lightVec = gl_LightSource[0].position.xyz - position.xyz; 
	lightVec = normalize(lightVec); 
	float cosinus = dot(normal, lightVec); 
	vec4 blubb = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * position * cosinus +( vec4(normal,1.0) * abs(sin(0.025*Time)) );
	//* texture2D(tex, gl_TexCoord[0].xy);
	//gl_FragColor = vec4(blubb.xyz,1);
	gl_FragColor = position*vec4(blubb.xyz,1);
} 
