#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

varying vec4 tf_data;
varying vec3 tf_position;
varying vec3 tf_normal;

uniform vec3 cameraPosition;

uniform float maxlength;
uniform float nearCameraDistance;
uniform float farCameraDistance;

int getNextVertex(int n) {
	return int(mod((n + 1), 3));
}

float normalizeCameraDistance(vec3 middlePoint) {

	float cameraDistance = distance(middlePoint, vec3(-cameraPosition.x,
			cameraPosition.y, -cameraPosition.z));

	if (cameraDistance >= farCameraDistance)
		return 1.0;
	if (cameraDistance <= nearCameraDistance)
		return 0.0;

	return cameraDistance / (farCameraDistance - nearCameraDistance);
}

void div0() {

	tf_normal = gl_TexCoordIn[0][0].xyz;
	tf_data = gl_FrontColorIn[0];

	tf_position = gl_PositionIn[0].xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[1][0].xyz;
	tf_data = gl_FrontColorIn[1];

	tf_position = gl_PositionIn[1].xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[2][0].xyz;
	tf_data = gl_FrontColorIn[2];

	tf_position = gl_PositionIn[2].xyz;
	EmitVertex();

	EndPrimitive();
}

void div1(int v0) {

	int v1 = getNextVertex(v0);
	int v2 = getNextVertex(v1);

	tf_normal = gl_TexCoordIn[v0][0].xyz;
	tf_data = gl_FrontColorIn[v0];

	tf_position = gl_PositionIn[v0].xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[v2][0].xyz;
	tf_data = gl_FrontColorIn[v2];

	tf_position = gl_PositionIn[v2].xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[v0][0].xyz + gl_TexCoordIn[v1][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[v0] + gl_FrontColorIn[v1]) * 0.5;

	tf_position = ((gl_PositionIn[v0] + gl_PositionIn[v1]) * 0.5).xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[v1][0].xyz;
	tf_data = gl_FrontColorIn[v1];

	tf_position = gl_PositionIn[v1].xyz;
	EmitVertex();

	EndPrimitive();
}

void div2(int v0) {

	int v1 = getNextVertex(v0);
	int v2 = getNextVertex(v1);

	tf_normal = gl_TexCoordIn[v1][0].xyz;
	tf_data = gl_FrontColorIn[v1];

	tf_position = gl_PositionIn[v1].xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[v1][0].xyz + gl_TexCoordIn[v2][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[v1] + gl_FrontColorIn[v2]) * 0.5;

	tf_position = ((gl_PositionIn[v1] + gl_PositionIn[v2]) * 0.5).xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[v0][0].xyz + gl_TexCoordIn[v1][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[v0] + gl_FrontColorIn[v1]) * 0.5;

	tf_position = ((gl_PositionIn[v0] + gl_PositionIn[v1]) * 0.5).xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[v2][0].xyz;
	tf_data = gl_FrontColorIn[v2];

	tf_position = gl_PositionIn[v2].xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[v0][0].xyz;
	tf_data = gl_FrontColorIn[v0];

	tf_position = gl_PositionIn[v0].xyz;
	EmitVertex();

	EndPrimitive();
}

void div3() {

	tf_normal = gl_TexCoordIn[0][0].xyz;
	tf_data = gl_FrontColorIn[0];

	tf_position = gl_PositionIn[0].xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[0][0].xyz + gl_TexCoordIn[1][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[0] + gl_FrontColorIn[1]) * 0.5;

	tf_position = ((gl_PositionIn[0] + gl_PositionIn[1]) * 0.5).xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[0][0].xyz + gl_TexCoordIn[2][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[0] + gl_FrontColorIn[2]) * 0.5;

	tf_position = ((gl_PositionIn[0] + gl_PositionIn[2]) * 0.5).xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[1][0].xyz + gl_TexCoordIn[2][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[1] + gl_FrontColorIn[2]) * 0.5;

	tf_position = ((gl_PositionIn[1] + gl_PositionIn[2]) * 0.5).xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[2][0].xyz;
	tf_data = gl_FrontColorIn[2];

	tf_position = gl_PositionIn[2].xyz;
	EmitVertex();
	EndPrimitive();

	tf_normal = (gl_TexCoordIn[0][0].xyz + gl_TexCoordIn[1][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[0] + gl_FrontColorIn[1]) * 0.5;

	tf_position = ((gl_PositionIn[0] + gl_PositionIn[1]) * 0.5).xyz;
	EmitVertex();

	tf_normal = (gl_TexCoordIn[1][0].xyz + gl_TexCoordIn[2][0].xyz) * 0.5;
	tf_data = (gl_FrontColorIn[1] + gl_FrontColorIn[2]) * 0.5;

	tf_position = ((gl_PositionIn[1] + gl_PositionIn[2]) * 0.5).xyz;
	EmitVertex();

	tf_normal = gl_TexCoordIn[1][0].xyz;
	tf_data = gl_FrontColorIn[1];

	tf_position = gl_PositionIn[1].xyz;
	EmitVertex();

	EndPrimitive();
}

void main(void) {

	/*Choose the right division according to the texture length and camera distance*/

	float textureLength = 0.0;
	int divisionType = 0;

	//vertex 0 and 1
	vec2 textureCoord1 = gl_FrontColorIn[0].xy;
	vec2 textureCoord2 = gl_FrontColorIn[1].xy;
	textureLength = distance(textureCoord1, textureCoord2);

	vec3 middlePoint = (gl_PositionIn[0].xyz + gl_PositionIn[1].xyz) * 0.5;
	float normalizedCameraDistance = normalizeCameraDistance(middlePoint);

	if (textureLength >= maxlength * normalizedCameraDistance) {
		divisionType += 1;
	}

	//vertex 1 and 2
	textureCoord1 = gl_FrontColorIn[1].xy;
	textureCoord2 = gl_FrontColorIn[2].xy;
	textureLength = distance(textureCoord1, textureCoord2);

	middlePoint = (gl_PositionIn[1].xyz + gl_PositionIn[2].xyz) * 0.5;
	normalizedCameraDistance = normalizeCameraDistance(middlePoint);

	if (textureLength >= maxlength * normalizedCameraDistance) {
		divisionType += 2;
	}

	//vertex 2 and 3
	textureCoord1 = gl_FrontColorIn[2].xy;
	textureCoord2 = gl_FrontColorIn[0].xy;
	textureLength = distance(textureCoord1, textureCoord2);

	middlePoint = (gl_PositionIn[2].xyz + gl_PositionIn[0].xyz) * 0.5;
	normalizedCameraDistance = normalizeCameraDistance(middlePoint);

	if (textureLength >= maxlength * normalizedCameraDistance) {
		divisionType += 4;
	}

	/*This operation can be performed in another way.
	 * if-else is used due to didactical reasons.*/
	if (divisionType == 0) {
		div0();
	} else {
	if (divisionType == 1) {
		div1(0);
	} else {
	if (divisionType == 2) {
		div1(1);
	} else {
	if (divisionType == 3) {
		div2(0);
	} else {
	if (divisionType == 4) {
		div1(2);
	} else {
	if (divisionType == 5) {
		div2(2);
	} else {
	if (divisionType == 6) {
		div2(1);
	} else {
	if (divisionType == 7) {
		div3();
	}}}}}}}}

	EndPrimitive();
}

