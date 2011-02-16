#version 330 core

uniform sampler2D height;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec2 in_Uv;

out vec2 uv;
out vec3 normalView;
out vec4 positionView;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
uniform vec2 uvoffset;

uniform float tiling;

//normalmap
out vec3 lightVec;
out vec3 eyeVec;
uniform vec4 lightPositionView;

float multiSampleHeight(vec2 position) {
    float h = texture(height, uv*0.1+position).x;
    h +=texture(height, uv*0.05+position).x;
    h +=texture(height, uv*0.01+position).x;
    return h;
}

void main(void) {
    //uv = (in_Uv + uvoffset)*tiling+vec2(tiling,tiling);
    uv = (in_Uv ) * tiling - vec2(.5, .5)*tiling + uvoffset;
    //uv = (in_Uv + uvoffset);
    
    //sample the height map multiple times
    float h = multiSampleHeight(vec2(0,0));
    
    float sampleSize = 0.005;
    float vectorSize = 0.05;
    
    vec3 binormal = normalize(
    	vec3(vectorSize,multiSampleHeight(vec2(0,sampleSize)),0) - 
    	vec3(-vectorSize,multiSampleHeight(vec2(0,-sampleSize)),0));
    vec3 tangent = normalize(
    	vec3(0,multiSampleHeight(vec2(sampleSize,0)),vectorSize) - 
    	vec3(0,multiSampleHeight(vec2(-sampleSize,0)),-vectorSize));
    vec3 vertexNormal = normalize(cross(tangent,binormal));
    /*
    vec3 vertexNormal = normalize(texture(normal, uv*0.1).rgb);
    vertexNormal = mix(vertexNormal, normalize(texture(normal, uv*0.05).rgb), 0.5);
    vertexNormal = mix(vertexNormal, normalize(texture(normal, uv*0.01).rgb), 0.5);
    */
    //p.y += h/tiling;
    normalView = NormalMatrix * vertexNormal;
    //normalView = vertexNormal;

    
	vec3 position = in_Vertex + normalize(in_Normal) * h/tiling;
    vec4 p = vec4(position,1);

	positionView = MVMatrix * p;
    gl_Position = MVPMatrix * p;
    
    //normalmap
    vec3 n = normalView; 
	vec3 t = NormalMatrix *tangent; 
	vec3 b = NormalMatrix *binormal;

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
