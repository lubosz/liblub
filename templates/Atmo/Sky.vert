{% extends "Atmo/Atmo.vert" %}
{% block linkage %}
out vec3 v3Direction;
out vec3 color1;
out vec3 color2;
out vec3 debugColor;
uniform mat4 MMatrix;
{% endblock %}

{% block main %}
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	//vec3 v3Pos = (MMatrix*vec4(in_Vertex,1)).xyz;
	vec3 v3Pos = in_Vertex;
	//vec3 localPosition = in_Vertex;
	vec3 v3Ray = v3Pos - cameraPosition;
	float fFar = length(v3Ray);
	//debugColor = fFar * vec3(1);
	v3Ray /= fFar;

{% if fromSpace %}
	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float B = 2.0 * dot(cameraPosition, v3Ray);
	float C = cameraHeight2 - outerRadius2;
debugColor = C * vec3(1);
	float fDet = max(0.0, B*B - 4.0 * C);
	float fNear = 0.5 * (-B - sqrt(fDet));

	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = cameraPosition + v3Ray * fNear;
	fFar -= fNear;
	float fStartAngle = dot(v3Ray, v3Start) / outerRadius;
	float fStartDepth = exp(-1.0 / scaleDepth);
	float fStartOffset = fStartDepth*scale(fStartAngle);
{% else %}
debugColor = vec3(0,0,1);
	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = cameraPosition;
	float fHeight = length(v3Start);
	float fDepth = exp(scaleOverScaleDepth * (innerRadius - cameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);
{% endif %}
debugColor = v3Pos;
	// Initialize the scattering loop variables
	float fSampleLength = fFar / samplesf;
	float scaledLength = fSampleLength * invSphereDistance;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	
	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i<samplesi; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(scaleOverScaleDepth * (innerRadius - fHeight));
		float fLightAngle = dot(lightPosition, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		vec3 v3Attenuate = exp(-fScatter * (invWavelength * rayleigh4Pi + mie4Pi));
		v3FrontColor += v3Attenuate * (fDepth * scaledLength);
		v3SamplePoint += v3SampleRay;
		//debugColor = length(v3SamplePoint)*vec3(1);
		//debugColor = (MMatrix*vec4(v3SamplePoint,1)).xyz;
		//debugColor = fHeight*vec3(1);
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	color2 = v3FrontColor * mieSun;
	color1 = v3FrontColor * (invWavelength * rayleighSun);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	v3Direction = cameraPosition - v3Pos;
	debugColor = color1;
	
{% endblock %}
