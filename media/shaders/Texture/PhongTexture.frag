#version 330 core

in vec3 position;
in vec3 normal;
in vec2 uv;

in float    LightIntensity;

out vec4 gl_FragColor;

uniform sampler2D myTexture;

uniform vec3 LightPosition;

void main() 
{ 

	vec3 normal = normalize(normal); 
	vec3 lightVec = LightPosition - position; 
	lightVec = normalize(lightVec); 
	float cosinus = dot(normal, lightVec); 
	//vec4 blubb = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse * position * cosinus +( vec4(normal,1.0) * abs(sin(0.025*Time)) );
	gl_FragColor = texture(myTexture, uv) * LightIntensity;
	//gl_FragColor = vec4(normal,1) * cosinus;

	//* texture2D(tex, gl_TexCoord[0].xy);
	//gl_FragColor = vec4(blubb.xyz,1);
} 
