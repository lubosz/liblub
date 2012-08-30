	// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = (MMatrix*vec4(in_Vertex,1)).xyz;
	//vec3 v3Pos = in_Vertex;
	vec3 v3Ray = v3Pos - cameraPosition;
	float fFar = length(v3Ray);
	v3Ray /= fFar;
	
{% if fromSpace %}
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float fNear = getNearIntersection(cameraPosition, v3Ray, cameraHeight2, outerRadius2);
	
	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = cameraPosition + v3Ray * fNear;
	fFar -= fNear;
	float fDepth = exp((innerRadius - outerRadius) / scaleDepth);
{% else %}
	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = cameraPosition;
	float fDepth = exp((innerRadius - cameraHeight) / scaleDepth);
{% endif %}

	float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
	float fLightAngle = dot(lightPosition, v3Pos) / length(v3Pos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / samplesf;
	float scaledLength = fSampleLength * invSphereDistance;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0);
	vec3 v3Attenuate = vec3(0);
	for(int i=0; i<samplesi; i++) {
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(scaleOverScaleDepth * (innerRadius - fHeight));
		if(attenuation){
			float fScatter = fDepth*fTemp - fCameraOffset;
			v3Attenuate = exp(-fScatter * (invWavelength * rayleigh4Pi + mie4Pi));
		} else {
			v3Attenuate = vec3(1);
		}
		v3FrontColor += v3Attenuate * (fDepth * scaledLength);
		v3SamplePoint += v3SampleRay;
	}
	color1 = v3FrontColor * (invWavelength * rayleighSun + mieSun);

	// Calculate the attenuation factor for the ground
	color2 = v3Attenuate;
	uv = in_Uv;