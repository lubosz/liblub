{% extends "base.vert" %}

{% block uniforms %}
uniform vec2 textureScale;
uniform vec2 bumpSpeed;

uniform vec3 eyePosition;

uniform float BumpScale;
uniform float time;
uniform float waveFreq;
uniform float waveAmp;

uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
uniform vec3 LightPosition;
{% endblock %}

{% block linkage %}

in vec3 in_Normal;
in vec3 in_Color;
in vec2 in_Uv;
out vec2 uv;

    out vec4 positionView;

out mat3 rotMatrix; //  transform from tangent to obj space
out vec2 bumpCoord0;
out vec2 bumpCoord1;
out vec2 bumpCoord2;
out vec3 normal;
out vec3 eyeVector;
out vec3 color;
uniform mat4 MMatrix;
// wave functions
struct Wave {
  float freq;  // 2*PI / wavelength
  float amp;   // amplitude
  float phase; // speed * 2*PI / wavelength
  vec2 dir;
};
{% endblock %}

{% block main %}
	#define NWAVES 2
	Wave wave[NWAVES];
	wave[0] = Wave( waveFreq, waveAmp, 0.5, vec2(-1, 0) );
	wave[1] = Wave( 3.0 * waveFreq, 0.33 * waveAmp, 1.7, vec2(-0.7, 0.7) );


    vec4 P = vec4(in_Vertex,1);

	// sum waves
	float ddx = 0.0, ddy = 0.0;
	float deriv;
	float angle;

	// wave synthesis using two sine waves at different frequencies and phase shift
	for(int i = 0; i<NWAVES; ++i)
	{
		angle = dot(wave[i].dir, P.xz) * wave[i].freq + time * wave[i].phase;
		//P.y += wave[i].amp * sin( angle );
		//P += normalize(vec4(in_Normal,1)) * wave[i].amp * sin( angle );
		// calculate derivate of wave function
		deriv = wave[i].freq * wave[i].amp * cos(angle);
		ddx -= deriv * wave[i].dir.x;
		ddy -= deriv * wave[i].dir.y;
	}

	// compute the 3x3 tranform from tangent space to object space
	// compute tangent basis
    vec3 T = normalize(vec3(1.0, ddy, 0.0)) * BumpScale;
    vec3 B = normalize(vec3(0.0, ddx, 1.0)) * BumpScale;
    vec3 N = normalize(vec3(ddx, 1.0, ddy));

	rotMatrix = mat3(T, B, N);

	gl_Position = MVPMatrix * P;
	//gl_Position     = MVPMatrix * vec4(in_Vertex,1);

	// calculate texture coordinates for normal map lookup

        /*
        bumpCoord0.xy = uvScaled;
        bumpCoord1.xy = uvScaled;
        bumpCoord2.xy = uvScaled;
        */
        vec2 uvScaled = in_Uv;
        bumpCoord0.xy = uvScaled + time * bumpSpeed;
        bumpCoord1.xy = uvScaled * 2.0 + time * bumpSpeed * 4.0;
        bumpCoord2.xy = uvScaled * 4.0 + time * bumpSpeed * 8.0;


	eyeVector = P.xyz - eyePosition; // eye position in vertex space

        color = in_Color;

        normal = normalize(NormalMatrix * in_Normal);

//        normal = normalize(in_Normal);
      positionView = MVMatrix * P;
        //color = vec3(1,0,0);
{% endblock %}
