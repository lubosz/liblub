#version 330 core

//uniform sampler2D Pass0;
uniform sampler2D RenderTexture;
uniform vec3 lightPositionView;

in vec2 uv;

out vec4 fragColor;

void main()
{
	float intensity=0.0;
	vec2 lightvec=uv-lightPositionView.xy;
	lightvec=lightvec*(1.0-clamp(length(lightvec),0.0,1.0));
	if((lightPositionView.z-1.0)>0.0){
		for(int i=1;i<30;i++){
			intensity+=texture2D(RenderTexture, uv-lightvec*0.045*float(i)).r*pow(length(lightvec),2.5);
		}
	}
	intensity+=texture2D(RenderTexture, uv).r;
	fragColor = vec4(vec3(intensity),1.0);
}

