{% extends "base.vert" %}

{% block header %}
in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;
out vec2 uv;

{% block linkage %}
{% endblock %}

uniform mat4 MVPMatrix;

uniform vec3 cameraPosition;		// The camera's current position
uniform vec3 lightPosition;		// The direction vector to the light source
uniform vec3 invWavelength;	// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float cameraHeight;	// The camera's current height
uniform float cameraHeight2;	// cameraHeight^2
uniform float outerRadius;		// The outer (atmosphere) radius
uniform float outerRadius2;	// outerRadius^2
uniform float innerRadius;		// The inner (planetary) radius
uniform float innerRadius2;	// innerRadius^2
uniform float rayleighSun;			// Kr * ESun
uniform float mieSun;			// Km * ESun
uniform float rayleigh4Pi;			// Kr * 4 * PI
uniform float mie4Pi;			// Km * 4 * PI
uniform float invSphereDistance;			// 1 / (outerRadius - innerRadius)
uniform float scaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)
uniform float scaleOverScaleDepth;	// scale / scaleDepth

const int samplesi = 20;
const float samplesf = 20.0;

float scale(float fCos) {
	float x = 1.0 - fCos;
	return scaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}
{% endblock %}