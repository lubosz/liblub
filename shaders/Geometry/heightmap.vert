#version 330 core

uniform sampler2D height;

in vec3 in_Vertex;
in vec3 in_Normal;
in vec3 in_Tangent;
in vec3 in_Bitangent;
in vec2 in_Uv;

out vec2 uv;
out vec3 normalView;
out vec4 positionView;

uniform mat4 MVPMatrix;
uniform mat4 MVMatrix;
uniform mat3 NormalMatrix;
uniform vec2 uvoffset;
uniform vec3 cameraPosition;

//uniform float zoom;

//normalmap
out vec3 lightVec;
out vec3 eyeVec;
uniform vec4 lightPositionView;
out float zoom;

float multiSampleHeight(vec2 position, float zoom) {
	float veryNearHeight = 0;
    float nearHeight = 0;
    float farHeight = 0;
    
    veryNearHeight +=texture(height, uv*0.1+position).x;
    veryNearHeight +=texture(height, uv*0.06+position).x;
    veryNearHeight +=texture(height, uv*0.05+position).x;
    veryNearHeight +=texture(height, uv*0.01+position).x;
    //veryNearHeight +=texture(height, uv*0.001+position).x;
    
    //nearHeight +=texture(height, uv*0.01+position).x;
   	nearHeight +=texture(height, uv*0.001+position).x;
   	nearHeight +=texture(height, uv*0.005+position).x;
   	
   	farHeight +=texture(height, uv*0.005+position).x;
    farHeight +=texture(height, uv*0.001+position).x;
    
    /*
    */
    /*
    */
    farHeight +=texture(height, uv*0.0005+position).x;
    farHeight +=texture(height, uv*0.0001+position).x;
    farHeight +=texture(height, uv*0.00005+position).x;
    farHeight +=texture(height, uv*0.00001+position).x;
    
    float h1 = mix(veryNearHeight, nearHeight, zoom);
    float h2 = mix(nearHeight, farHeight, zoom);
    return mix(h1, h2, zoom);
    //return texture(height, uv*(1-zoom)+position).x/2.0;
}

void main(void) {
	//float dist = distance(cameraPosition,in_Vertex.xyz);
	//float dist = length(cameraPosition);
	float dist = cameraPosition.y;
    //float blend = max(0, min((dist) / 100.0, 1));
    //uv = (in_Uv + uvoffset)*zoom+vec2(zoom,zoom);
    zoom = max(0.01, min((dist) / 20.0, 0.9));
    float uvZoom = 1000.1 * zoom;
    float sphereZoom = zoom;
    float displacementZoom = zoom * 4;
    //uv = (in_Uv ) * blend - vec2(.5, .5)*blend + uvoffset;
    uv = (in_Uv ) * uvZoom - vec2(.5, .5)*uvZoom ;
    //+ cameraPosition.xy;
    
    //uv = (in_Uv + uvoffset);
    
    //sample the height map multiple times
    float h = multiSampleHeight(vec2(0,0),zoom);
    
    float sampleSize = 0.005;
    float vectorSize = 0.05;
    
    vec3 binormal = normalize(
    	vec3(vectorSize,multiSampleHeight(vec2(0,sampleSize),zoom),0) - 
    	vec3(-vectorSize,multiSampleHeight(vec2(0,-sampleSize),zoom),0));
    vec3 tangent = normalize(
    	vec3(0,multiSampleHeight(vec2(sampleSize,0),zoom),vectorSize) - 
    	vec3(0,multiSampleHeight(vec2(-sampleSize,0),zoom),-vectorSize));
    vec3 vertexNormal = normalize(cross(tangent,binormal));
    /*
    vec3 vertexNormal = normalize(texture(normal, uv*0.1).rgb);
    vertexNormal = mix(vertexNormal, normalize(texture(normal, uv*0.05).rgb), 0.5);
    vertexNormal = mix(vertexNormal, normalize(texture(normal, uv*0.01).rgb), 0.5);
    */
    //p.y += h/zoom;
    normalView = NormalMatrix * vertexNormal;
    //normalView = vertexNormal;



	//vec3 displacement = normalize(in_Vertex) * 0.04 * h * (1.0-sphereZoom);
	vec3 displacement = mix(vec3(0,3,0),in_Normal,sphereZoom*2) * 0.06 * h * (1.0-sphereZoom);
	
	
	//vec3 displacement = normalize(mix(in_Normal,in_Vertex,sphereZoom)) * h/displacementZoom;
	//vec3 displacement = normalize(mix(in_Normal,in_Vertex,blend)) * h/blend;
	//displacement = vec3(0);
    
    //vec3 position = normalize(in_Vertex);
	//vec3 position = in_Vertex + displacement;
	//vec3 position = mix(in_Vertex,normalize(in_Vertex),sphereZoom)+ displacement;
	vec3 position = mix(vec3(in_Vertex.x,0,in_Vertex.z),in_Vertex,sphereZoom)+ displacement;
	
	
	
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
