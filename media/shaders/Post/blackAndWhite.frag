#version 150 core

uniform sampler2D FBOTex;
uniform vec3 LightPosition;

in vec2 TexCoord;

out vec4 FragColor;

void main()
{
	float intensity=0.0;
	vec2 lightvec=TexCoord-LightPosition.xy;
	lightvec=lightvec*(1.0-clamp(length(lightvec),0.0,1.0));
	if((LightPosition.z-1.0)>0.0){
		for(int i=1;i<30;i++){
			intensity+=texture2D(FBOTex, TexCoord-lightvec*0.045*float(i)).r*pow(length(lightvec),2.5);
		}
	}
	intensity+=texture2D(FBOTex, TexCoord).r;
	FragColor = vec4(vec3(intensity),1.0);
}

