#version 330 core

in vec4 positionCamView;
in vec3 normalCamView;
in vec2 uv;

out vec4 fragColor;

// This define the value to move one pixel left or right
uniform float xPixelOffset ;
	
// This define the value to move one pixel up or down
uniform float yPixelOffset ;
	
uniform sampler2DShadow shadowMap;

uniform vec3 lightPositionView;
//uniform sampler2D shadowMap;
uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

float lookup( vec2 offSet,vec4 shadowTexCoord){
	// Values are multiplied by ShadowCoord.w because shadow2DProj does a W division for us.
	return textureProj(shadowMap, 
		shadowTexCoord 
		+ vec4(
			offSet.x * xPixelOffset * shadowTexCoord.w, 
			offSet.y * yPixelOffset * shadowTexCoord.w, 
			0, 
			0
		) 
	);
}

void main(){		
	float shadow ;
	
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionCamView;
		
	// Avoid counter shadow
	//if (shadowTexCoord.w > 1.0){
	/*
		float x,y;
		for (y = -1.5 ; y <=1.5 ; y+=1.0)
			for (x = -1.5 ; x <=1.5 ; x+=1.0)
				shadow += lookup(vec2(x,y),shadowTexCoord);
			
		shadow /= 16.0 ;
		*/
		
		// 8x8 kernel PCF
					
		float x,y;
		for (y = -3.5 ; y <=3.5 ; y+=1.0)
			for (x = -3.5 ; x <=3.5 ; x+=1.0)
				shadow += lookup(vec2(x,y),shadowTexCoord);
					
		shadow /= 64.0 ;
		
		
	
	//}
	fragColor = (shadow+0.2) * vec4(1,1,1,1);
	  
}

