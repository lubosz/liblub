{% extends "Atmo/Atmo.vert" %}
{% block linkage %}
out vec3 color;
{% endblock %}

{% block main %}
	// Get the ray from the camera to the vertex and its length
	vec3 v3Pos = in_Vertex;
	vec3 v3Ray = v3Pos - cameraPosition;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the farther intersection of the ray with the outer atmosphere (which is the far point of the ray passing through the atmosphere)
	fFar = getFarIntersection(cameraPosition, v3Ray, cameraHeight2, outerRadius2);
{% if fromSpace %}
	float fNear = 0.5 * (-B - sqrt(fDet));
	vec3 v3Start = cameraPosition + v3Ray*fNear;
	fFar -= fNear;
{% else %}
	vec3 v3Start = cameraPosition;
{% endif %}

	// Calculate attenuation from the camera to the top of the atmosphere toward the vertex
	float fHeight = length(v3Start);
	float fDepth = exp(scaleOverScaleDepth * (innerRadius - cameraHeight));
	float fAngle = dot(v3Ray, v3Start) / fHeight;
	float fScatter = fDepth*scale(fAngle);
	color = exp(-fScatter * (invWavelength * rayleigh4Pi + mie4Pi));
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
	uv = in_Uv;
{% endblock %}
