// BillboardVert.glsl
//

uniform vec3 v3CameraPos;		// The camera's current position
uniform vec3 v3CameraView;		// The camera's current view vector
uniform vec3 v3CameraUp;		// The camera's current up vector
uniform float fScale;

void main()
{
	//float fDistance = distance(gl_Vertex.xyz, v3CameraPos);
	vec3 v3Up = normalize(gl_Vertex.xyz);
	vec3 v3Right = cross(v3Up, -v3CameraView);
	v3Right = normalize(v3Right);

	vec3 v3Pos = gl_Vertex.xyz + gl_Normal.x*fScale * v3Right + gl_Normal.y * v3Up*fScale;
	gl_Position = gl_ModelViewProjectionMatrix * vec4(v3Pos.x, v3Pos.y, v3Pos.z, 1.0);
	gl_FrontColor = gl_Color;
	//gl_FrontColor.a = 10.0 / fDistance;

	gl_TexCoord[0].s = (gl_Normal.x + 0.5) * 256.0;
	gl_TexCoord[0].t = (0.5 - gl_Normal.y) * 128.0;
}

