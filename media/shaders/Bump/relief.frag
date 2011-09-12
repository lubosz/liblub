#version 330 core

//precision highp float;

in vec4 vpos;
in vec3 normal;
in vec3 tangent;
in vec2 texcoord;

uniform sampler2D reliefMap;	// rm texture map
uniform sampler2D colortex;		// color texture map
uniform vec4 lightPositionView;	// light position in view space
uniform vec4 ambient;			// ambient color
uniform vec4 diffuse;			// diffuse color
uniform vec4 specular;			// specular color
uniform vec2 planes;			// near and far planes info
uniform float tile;				// tile factor
uniform float depth;			// scale factor for height-field depth

//const int linearSearchSteps = 10;
//const int binarySearchSteps = 5;
const int linearSearchSteps = 20;
const int binarySearchSteps = 10;

out vec4 fragColor;

float saturate(float value){
	return clamp(value, 0.0, 1.0);
}

float ray_intersect_rm(vec2 dp, vec2 ds){

	float depthStep=1.0/linearSearchSteps;
	float size=depthStep; // current size of search window
	float depth=0.0; // current depth position
	// best match found (starts with last position 1.0)
	float bestDepth=1.0;
	// search from front to back for first point inside the object
	for ( int i=0; i< linearSearchSteps-1;i++){
		depth+=size;
		vec4 t=texture(reliefMap,dp+ds*depth);
		if (bestDepth>0.996) // if no depth found yet
			if (depth >= t.w)
				bestDepth=depth; // store bestDepth
	}
	depth = bestDepth;
	// search around first point (depth) for closest match
	for ( int i=0; i < binarySearchSteps; i++) {
		size*=0.5;
		vec4 t=texture(reliefMap,dp+ds*depth);
		if (depth >= t.w) {
			bestDepth = depth;
			depth -= 2*size;
		}
		depth+=size;
	}
	return bestDepth;
}

void main(){

	vec4 t,c; 
	vec3 p,v,l,s; 
	vec2 dp,ds,uv; 
	float d;
	
	vec3 binormal = -cross(normal,tangent);

	// ray intersect in view direction
	p = vpos.xyz; // pixel position in eye space
	v = normalize(p); // view vector in eye space
	// view vector in tangent space
	s = 
	normalize(
		vec3(
			dot(v, tangent),
			dot(v, -binormal),
			dot(v, normal)
		)
	);
	// size and start position of search in texture space
	//ds = s.xy*depth/s.z;
	ds = s.xy*depth;
	dp = texcoord*tile;
	
	// get intersection distance
	d = ray_intersect_rm(dp,ds);
	// get normal and color at intersection point
	uv=dp+ds*d;
	//uv = ds;
	//uv = dp*d;
	t=texture(reliefMap,uv);
	c=texture(colortex,uv);
	
	t.xyz=t.xyz*2.0-1.0; // expand normal to eye space
	t.xyz=normalize(t.x*tangent+t.y*binormal+t.z*normal);
	// compute light direction
	p += v*d*s.z;
	l=normalize(p-lightPositionView.xyz);
	
	#ifdef RM_DEPTHCORRECT
		// planes.x=-far/(far-near); planes.y =-far*near/(far-near);
		float depth=((planes.x*p.z+planes.y)/-p.z);
	#endif
	
	// compute diffuse and specular terms
	float att=saturate(dot(-l,normal));
	float diff=saturate(dot(-l,t.xyz));
	float spec=saturate(dot(normalize(-l-v),t.xyz));
	
	//fragColor = c;
	fragColor = ambient * c;
	
	#ifdef RM_SHADOWS
		// ray intersect in light direction
		dp+= ds*d; // update position in texture space
		// light direction in texture space
		s = normalize(vec3(dot(l,tangent.xyz),
		dot(l,binormal.xyz),dot(normal,-l)));
		ds = s.xy*depth/s.z;
		dp-= ds*d; // entry point for light ray in texture space
		// get intresection distance from light ray
		float dl = ray_intersect_rm(dp,ds.xy);
		if (dl<d-0.05) // if pixel in shadow
			{ diff*=dot(ambient.xyz,vec3(1.0,1.0,1.0))*0.3333; spec=0; }
	#endif
	
	fragColor.xyz+=att*(c.xyz*diffuse.xyz*diff+specular.xyz*pow(spec,specular.w));
	fragColor.w=1.0;
}
