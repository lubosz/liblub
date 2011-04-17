{% extends "base.vert" %}
{% block linkage %}
in vec2 in_Uv;
out vec2 uv;
out vec3 color1;
out vec3 color2;
{% endblock %}

{% block uniforms %}
uniform mat4 MVPMatrix;
{% include "Atmo/AtmoUniforms.vert" %}
{% include "Atmo/ScaleFunction.vert" %}
{% endblock %}

{% block main %}
	// Get the ray from the camera to the vertex, and its length (which is the far point of the ray passing through the atmosphere)
	vec3 v3Pos = in_Vertex;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 v3Start = v3CameraPos;
	float fDepth = exp((fInnerRadius - fCameraHeight) / fScaleDepth);
	float fCameraAngle = dot(-v3Ray, v3Pos) / length(v3Pos);
	float fLightAngle = dot(v3LightPos, v3Pos) / length(v3Pos);
	float fCameraScale = scale(fCameraAngle);
	float fLightScale = scale(fLightAngle);
	float fCameraOffset = fDepth*fCameraScale;
	float fTemp = (fLightScale + fCameraScale);

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0);
	vec3 v3Attenuate = vec3(0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fScatter = fDepth*fTemp - fCameraOffset;
		v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	color1 = v3FrontColor * (v3InvWavelength * fKrESun + fKmESun);

	// Calculate the attenuation factor for the ground
	color2 = v3Attenuate;

	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	//gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
	//gl_TexCoord[1] = gl_TextureMatrix[1] * gl_MultiTexCoord1;
	uv = in_Uv;
{% endblock %}
