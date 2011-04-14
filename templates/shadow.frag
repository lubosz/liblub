uniform sampler2DShadow shadowMap;
uniform mat4 camViewToShadowMapMatrix; //bias*perspLight*viewLight*(viewCam⁻1)

#ifdef usePCF
// This define the value to move one pixel left or right
uniform float xPixelOffset ;
// This define the value to move one pixel up or down
uniform float yPixelOffset ;

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
#endif

float getShadow() {
#ifdef usePCF
// 8x8 kernel PCF
	vec4 shadowTexCoord = camViewToShadowMapMatrix * positionView;
	float shadow = 0;	
	float x,y;
	for (y = -3.5 ; y <=3.5 ; y+=1.0)
		for (x = -3.5 ; x <=3.5 ; x+=1.0)
			shadow += lookup(vec2(x,y),shadowTexCoord);
				
	shadow /= 64.0;
	shadow += 0.2;
#else
	#ifdef receiveShadows
		vec4 shadowTexCoord = camViewToShadowMapMatrix * positionView;
		float shadow = textureProj(shadowMap, shadowTexCoord);
	#endif	
#endif
  return shadow;
}
