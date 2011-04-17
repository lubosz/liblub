{% extends "Atmo/Atmo.vert" %}
{% block linkage %}
out vec3 color;
{% endblock %}

{% block main %}
	// Get the ray from the camera to the vertex and its length
	vec3 v3Pos = in_Vertex;
	vec3 v3Ray = v3Pos - v3CameraPos;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the farther intersection of the ray with the outer atmosphere (which is the far point of the ray passing through the atmosphere)
	float B = 2.0 * dot(v3CameraPos, v3Ray);
	float C = fCameraHeight2 - fOuterRadius2;
	float fDet = max(0.0, B*B - 4.0 * C);
	fFar = 0.5 * (-B + sqrt(fDet));

	// Calculate attenuation from the camera to the top of the atmosphere toward the vertex
	vec3 v3Start = v3CameraPos;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fCameraHeight));
	float fAngle = dot(v3Ray, v3Start) / fHeight;
	float fScatter = fDepth*scale(fAngle);
	color = exp(-fScatter * (v3InvWavelength * fKr4PI + fKm4PI));
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	uv = in_Uv;
{% endblock %}
