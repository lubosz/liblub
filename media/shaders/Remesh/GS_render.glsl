#version 120
#extension GL_EXT_geometry_shader4 : enable
#extension GL_EXT_gpu_shader4 : enable

varying vec4 tf_data;
varying vec3 tf_position;
varying vec3 tf_normal;

void main(void) {
	tf_normal = vec3(0.0, 0.0, 1.0);
	for (int i=0; i<3; i++) {
		tf_data = gl_FrontColorIn[i];
		tf_position=gl_PositionIn[i].xyz;
		EmitVertex();
	}

	EndPrimitive();
}

