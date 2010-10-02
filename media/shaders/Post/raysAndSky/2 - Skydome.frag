#version 330 core

uniform sampler2D SkydomeTexture;
uniform sampler2D RenderTexture;

//uniform sampler2D Pass1;
uniform vec3 lightPositionView;
uniform mat4 invProjView;

in vec2 uv;

out vec4 fragColor;

vec3 result=vec3(0.0,0.0,0.0);
vec3 suncolor=vec3(0.9,0.9,0.63);

void main(void){
	vec2 lightvec=uv-lightPositionView.xy;
	float lightdist=length(lightvec*vec2(1.0,0.444));
	vec3 suncolor=(1.0-pow(lightdist,0.1))*suncolor;
	if(lightdist<0.02) suncolor=suncolor*pow((0.02-lightdist)/0.02+1.0,2.0);
	if(lightPositionView.z-1.0<0.0) suncolor=vec3(0.0);
	
	//float intensity=texture2D(GLGE_PASS1,uv).r;
	float intensity= 0.5;
	
	vec4 skycoord = invProjView*vec4(vec3((uv-0.5)*2.0,0.0),1.0);
	skycoord=invProjView*vec4(vec3((uv-0.5)*2.0,1.0),1.0);
	skycoord.xyz=normalize(skycoord.xyz/skycoord.w);
	
	result = 	max(suncolor,0.0) / 0.4
				+texture(SkydomeTexture,
					(skycoord.xy/(1.4+skycoord.z)+1.0)
					/2.0
					).rgb
					*intensity
				+texture(RenderTexture,uv).rgb*(1.0-intensity);
	vec3 col=texture2D(RenderTexture,uv).rgb+vec3(0.1,0.1,0.1);
	if(intensity<1.0) result=suncolor*dot(normalize(lightPositionView),vec3(0.0,0.0,1.0))*intensity*7.0+vec3(pow(col.r,2.5),pow(col.g,2.5),pow(col.b,2.5));	
	fragColor = vec4(result,1.0);
	
	fragColor = texture(SkydomeTexture,(skycoord.xy/(skycoord.z+1.4)+1.0)/2.0 );
}	
