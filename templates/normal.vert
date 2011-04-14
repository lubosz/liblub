in vec3 in_Normal;
out vec3 normalView;
uniform mat3 NormalMatrix;
uniform vec4 lightPositionView;

void tangentSpace(vec4 positionView) {
	vec3 n = normalize(NormalMatrix * in_Normal);
	vec3 t = normalize(NormalMatrix * in_Tangent);
	vec3 b = normalize(NormalMatrix * in_Bitangent);

	vec3 lightDirection = lightPositionView.xyz - positionView.xyz;

	lightVec = vec3(
		dot(lightDirection, t),
		dot(lightDirection, b),
		dot(lightDirection, n)
	);

	eyeVec.x = dot(-positionView.xyz, t);
	eyeVec.y = dot(-positionView.xyz, b);
	eyeVec.z = dot(-positionView.xyz, n);
}
