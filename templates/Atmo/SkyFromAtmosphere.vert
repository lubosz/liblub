{% extends "base.vert" %}
{% block linkage %}
in vec2 in_Uv;
out vec2 uv;
out vec3 v3Direction;
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
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth*scale(fStartAngle);

	// Initialize the scattering loop variables
	//gl_FrontColor = vec4(0.0, 0.0, 0.0, 0.0);
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	vec3 v3SampleRay = v3Ray * fSampleLength;
	vec3 v3SamplePoint = v3Start + v3SampleRay * 0.5;
	
	//vec4 lightPos = MVMatrix * vec4(v3LightPos,1);

	// Now loop through the sample rays
	vec3 v3FrontColor = vec3(0.0, 0.0, 0.0);
	for(int i=0; i<nSamples; i++)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth*(scale(fLightAngle) - scale(fCameraAngle)));
		vec3 v3Attenuate = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength);
		v3SamplePoint += v3SampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	color2 = v3FrontColor * fKmESun;
	color1 = v3FrontColor * (v3InvWavelength * fKrESun);
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	v3Direction = v3CameraPos - v3Pos;
{% endblock %}
