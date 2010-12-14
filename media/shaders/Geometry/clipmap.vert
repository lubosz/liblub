#version 330 core

uniform sampler2D heightMap;
uniform sampler2D vertexNormalMap;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec2 in_Uv;

out vec2 uv;
out vec4 positionView;
out vec3 normalView;

uniform mat4 MVMatrix;
uniform mat4 MVPMatrix;
uniform mat3 NormalMatrix;

uniform vec4 lightPositionView;

void main(void) {
    uv = in_Uv;
	//normalView = normalize(NormalMatrix * texture(heightMap, uv).rgb);
	normalView = normalize(NormalMatrix * vec3(0,1,0));
    
    float height = texture(heightMap, uv).x;
    vec4 position = vec4(in_Vertex,1);
    position.y += height;
    //float heightScale = mix(height.x,height.y,height.z);
    //position += in_Normal * heightScale * 0.2;
    //position -= in_Normal * 0.2;
    //position += normalize(in_Normal * heightScale) * 0.2;

    positionView = MVMatrix * position;
    gl_Position = MVPMatrix * position;
}
