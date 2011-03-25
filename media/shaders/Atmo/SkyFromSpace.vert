#version 330 core

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec3 cameraPosition;	// The camera's current position
uniform vec3 lightPosition;		// The direction vector to the light source

uniform float cameraHeight;		// The camera's current height
uniform float outerRadius;		// The outer (atmosphere) radius
uniform float innerRadius;		// The inner (planetary) radius
uniform vec3 invWavelength;		// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float krESun;			// Kr * ESun
uniform float kmESun;			// Km * ESun
uniform float kr4PI;			// Kr * 4 * PI
uniform float km4PI;			// Km * 4 * PI
/*
uniform vec3 wavelength;		// 1 / pow(wavelength, 4) for the red, green, and blue channels
uniform float ESun;
uniform float kr;
uniform float km;
*/
uniform float scaleDepth;		// The scale depth (i.e. the altitude at which the atmosphere's average density is found)

const float pi = 3.14159f;
const int nSamples = 20;
const float fSamples = 20.0;

/*
const float cameraHeight = 10.0;
vec3 invWavelength = vec3(1.0 / pow(wavelength.x,4.0),1.0 / pow(wavelength.y,4.0),1.0 / pow(wavelength.z,4.0));
float krESun = kr * ESun;
float kmESun = km * ESun;
float kr4PI = kr * 4 * pi;
float km4PI = km * 4 * pi;
*/
float cameraHeight2 = cameraHeight * cameraHeight;
float outerRadius2 = outerRadius * outerRadius;
float innerRadius2 = innerRadius * innerRadius;
float scale = 1.0 / (outerRadius - innerRadius);
float scaleOverScaleDepth = scale / scaleDepth;


out vec3 direction;
out vec3 color1;
out vec3 color2;


float scaleMe(float cosinus) {
	float x = 1.0 - cosinus;
	return scaleDepth * exp(-0.00287 + x*(0.459 + x*(3.83 + x*(-6.80 + x*5.25))));
}

void main(void)
{
	// Get the ray from the camera to the vertex and its length (which is the far point of the ray passing through the atmosphere)
	//vec3 position = (MVMatrix * vec4(in_Vertex,1.0)).xyz;
	vec3 position = in_Vertex;
	vec3 ray = position - cameraPosition;
	float far = length(ray);
	ray /= far;

	// Calculate the closest intersection of the ray with the outer atmosphere (which is the near point of the ray passing through the atmosphere)
	float B = 2.0 * dot(cameraPosition, ray);
	float C = cameraHeight2 - outerRadius2;
	float det = max(0.0, B*B - 4.0 * C);
	float near = 0.5 * (-B - sqrt(det));

	// Calculate the ray's starting position, then calculate its scattering offset
	vec3 start = cameraPosition + ray * near;
	far -= near;
	float startAngle = dot(ray, start) / outerRadius;
	float startDepth = exp(-1.0 / scaleDepth);
	float startOffset = startDepth*scaleMe(startAngle);

	// Initialize the scattering loop variables
	float sampleLength = far / fSamples;
	float scaledLength = sampleLength * scale;
	vec3 sampleRay = ray * sampleLength;
	vec3 samplePoint = start + sampleRay * 0.5;

	// Now loop through the sample rays
	vec3 frontColor = vec3(0.2, 0.2, 0.5);
	//vec3 frontColor = vec3(0, 0, 0);
	for (int i=0; i < nSamples; i++) {
		float height = length(samplePoint);
		float depth = exp(scaleOverScaleDepth * (innerRadius - height));
		float lightAngle = dot(lightPosition, samplePoint) / height;
		float cameraAngle = dot(ray, samplePoint) / height;
		float scatter = (startOffset + depth*(scaleMe(lightAngle) - scaleMe(cameraAngle)));
		vec3 attenuate = exp(-scatter * (invWavelength * kr4PI + km4PI));
		frontColor += attenuate * (depth * scaledLength);
		samplePoint += sampleRay;
	}

	// Finally, scale the Mie and Rayleigh colors and set up the varying variables for the pixel shader
	color1 = frontColor * (invWavelength * krESun);
	color2 = frontColor * kmESun;
	direction = cameraPosition - position;
	gl_Position = MVPMatrix * vec4(in_Vertex,1);
}
