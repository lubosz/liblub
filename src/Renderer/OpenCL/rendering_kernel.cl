/*
Copyright (c) 2009 David Bucciarelli (davibu@interfree.it)

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#define GPU_KERNEL

#include "src/Renderer/OpenCL/renderconfig.h"

#define BOUNDING_RADIUS_2 4.f
#define ESCAPE_THRESHOLD 1e1f
#define DELTA 1e-4f

float3 triplexPow(float3 vec, unsigned n) {
    float r = sqrt(vec.x*vec.x + vec.y * vec.y + vec.z * vec.z);
    float theta = 0;
    if (vec.x != 0)
        theta = n * atan(vec.y/vec.x);
    float phi = 0;
    if (r != 0)
        phi = n * asin(vec.z/r);
        
    return pow(r,n) * (
      (float3) (
        cos(theta)*cos(phi),
        sin(theta)*sin(phi),
        -sin(phi)
      )
    );
}

float norm(float3 in) {
    return dot(in, in);
}

float absolute(float3 in) {
    return sqrt(dot(in, in));
}

float3 triplexMult(float3 vec1, float3 vec2) {
    float r1 = sqrt(vec1.x*vec1.x + vec1.y*vec1.y);
    float r2 = sqrt(vec2.x*vec2.x + vec2.y*vec2.y);
    float a = 1.0 - vec1.z * vec2.z/(r1 * r2);

    float3 vector = (float3)(
                a * (vec1.x* vec2.x - vec1.y* vec2.y),
                a * (vec2.x* vec1.y - vec1.x* vec2.y),
                -(r2 * vec1.z + r1 * vec2.z));
    return vector;
}

float distanceEstimate(float3 c, int n, unsigned maxIter) {
    float3 p = (float3)(0.0, 0.0, 0.0);
    float3 dp = (float3)(1.0, 0.0, 0.0);
    const float3 dir = (float3)(1.0, 0.0, 0.0);
    float r;
    float dr = 1;
    int i = 0;
    bool loop = true;

    while(loop) {
        float3 foo = triplexPow(p, n);
        p =  foo + c;
        r = absolute(p);
        i++;
        loop = (i < maxIter && r < 2);
        if(loop) {
            dp = n * triplexMult(dp, triplexPow(p, n - 1)) + dir;
            dr = absolute(dp);
        }
    }

    return 2 * r * log(r) / dr / 4.0;

}

int mandelbulb(float3 c, int max) {
    float3 p = (float3)(0, 0, 0);
    int i = 0;
    while(i < max && norm(p) < 4) {
        p = triplexPow(p, 8) + c;
        i++;
    }
    return i;
}

static float4 QuatMult(const float4 q1, const float4 q2) {
	float4 r;

	// a1a2 - b1b2 - c1c2 - d1d2
	r.x = q1.x * q2.x - q1.y * q2.y - q1.z * q2.z - q1.w * q2.w;
	// a1b2 + b1a2 + c1d2 - d1c2
	r.y = q1.x * q2.y + q1.y * q2.x + q1.z * q2.w - q1.w * q2.z;
	// a1c2 - b1d2 + c1a2 + d1b2
	r.z = q1.x * q2.z - q1.y * q2.w + q1.z * q2.x + q1.w * q2.y;
	// a1d2 + b1c2 - c1b2 + d1a2
	r.w = q1.x * q2.w + q1.y * q2.z - q1.z * q2.y + q1.w * q2.x;

	return r;
}

static float4 QuatSqr(const float4 q) {
	float4 r;

	r.x = q.x * q.x - q.y * q.y - q.z * q.z - q.w * q.w;
	r.y = 2.f * q.x * q.y;
	r.z = 2.f * q.x * q.z;
	r.w = 2.f * q.x * q.w;

	return r;
}

static void IterateIntersect(float4 *q, float4 *qp,
		const float4 c, const uint maxIterations) {
	float4 q0 = *q;
	float4 qp0 = *qp;

	for (uint i = 0; i < maxIterations; ++i) {
		qp0 = 2.f * QuatMult(q0, qp0);
		q0 = QuatSqr(q0) + c;

		if (dot(q0, q0) > ESCAPE_THRESHOLD)
			break;
	}

	*q = q0;
	*qp = qp0;
}


static float IterateIntersectMandelBulb(float4 *q, float4 *qp,
		const float4 c, const uint maxIterations) {

  float4 q0 = *q;

  float z = 1.1;
  float distance = 1.1;

  unsigned iter = 0;
  while(distance > 0.001 && z >= -0.1 && iter <= maxIterations) {
    distance = distanceEstimate((float3)(q0.x, q0.y, q0.z), 8, maxIterations);
    z -= distance;
    iter++;
  }
  
  return distance;
  //return (iter == maxIterations);

}

static float IntersectJulia(const float4 eyeRayOrig, const float4 eyeRayDir,
		const float4 c, const uint maxIterations, const float epsilon,
		float4 *hitPoint, uint *steps) {
	float dist;
	float4 r0 = eyeRayOrig;

	uint s = 0;
	do {
		float4 z = r0;
		float4 zp = (float4) (1.f, 0.f, 0.f, 0.f);

		//IterateIntersect(&z, &zp, c, maxIterations);
    float dist2 = IterateIntersectMandelBulb(&z, &zp, c, maxIterations);
		const float normZP = length(zp);

		// We are inside
		//if (normZP == 0.f)
		if (dist2 > 1.1)
			break;

		const float normZ = length(z);
		dist = 0.5f * normZ * log(normZ) / normZP;

		r0 += eyeRayDir * dist;
		s++;
	} while ( (dist > epsilon) && (dot(r0, r0) < BOUNDING_RADIUS_2));

	*hitPoint = r0;
	*steps = s;
	return dist;
}

#define WORLD_RADIUS 1000.f
#define WORLD_CENTER ((float4)(0.f, -WORLD_RADIUS - 2.f, 0.f, 0.f))
float IntersectFloorSphere(const float4 eyeRayOrig, const float4 eyeRayDir) {
	const float4 op = WORLD_CENTER - eyeRayOrig;
	const float b = dot(op, eyeRayDir);
	float det = b * b - dot(op, op) + WORLD_RADIUS * WORLD_RADIUS;

	if (det < 0.f)
		return -1.f;
	else
		det = sqrt(det);

	float t = b - det;
	if (t > 0.f)
		return t;
	else {
		// We are inside, avoid the hit
		return -1.f;
	}
}

float IntersectBoundingSphere(const float4 eyeRayOrig, const float4 eyeRayDir) {
	const float4 op = -eyeRayOrig;
	const float b = dot(op, eyeRayDir);
	float det = b * b - dot(op, op) + BOUNDING_RADIUS_2;

	if (det < 0.f)
		return -1.f;
	else
		det = sqrt(det);

	float t = b - det;
	if (t > 0.f)
		return t;
	else {
		t = b + det;

		if (t > 0.f) {
			// We are inside, start from the ray origin
			return 0.0f;
		} else
			return -1.f;
	}
}

static float4 NormEstimate(const float4 p, const float4 c,
		const float delta, const uint maxIterations) {
	float4 N;
	float4 qP = p;
	float gradX, gradY, gradZ;

	float4 gx1 = qP - (float4)(DELTA, 0.f, 0.f, 0.f);
	float4 gx2 = qP + (float4)(DELTA, 0.f, 0.f, 0.f);
	float4 gy1 = qP - (float4)(0.f, DELTA, 0.f, 0.f);
	float4 gy2 = qP + (float4)(0.f, DELTA, 0.f, 0.f);
	float4 gz1 = qP - (float4)(0.f, 0.f, DELTA, 0.f);
	float4 gz2 = qP + (float4)(0.f, 0.f, DELTA, 0.f);

	for (uint i = 0; i < maxIterations; ++i) {
		gx1 = QuatSqr(gx1) + c;
		gx2 = QuatSqr(gx2) + c;
		gy1 = QuatSqr(gy1) + c;
		gy2 = QuatSqr(gy2) + c;
		gz1 = QuatSqr(gz1) + c;
		gz2 = QuatSqr(gz2) + c;
	}

	gradX = length(gx2) - length(gx1);
	gradY = length(gy2) - length(gy1);
	gradZ = length(gz2) - length(gz1);

	N = normalize((float4)(gradX, gradY, gradZ, 0.f));

	return N;
}

static float4 Phong(const float4 light, const float4 eye, const float4 pt,
		const float4 N, const float4 diffuse) {
	const float4 ambient = (float4) (0.05f, 0.05f, 0.05f, 0.f);
	float4 L = normalize(light - pt);
	float NdotL = dot(N, L);
	if (NdotL < 0.f)
		return diffuse * ambient;

	const float specularExponent = 30.f;
	const float specularity = 0.65f;

	float4 E = normalize(eye - pt);
	float4 H = (L + E) * (float)0.5f;

	return diffuse * NdotL +
			specularity * pow(dot(N, H), specularExponent) +
			diffuse * ambient;
}

__kernel void JuliaGPU(
	__global float *pixels,
	const __global RenderingConfig *config,
	const int enableAccumulation,
	const float sampleX,
	const float sampleY) {
    const int gid = get_global_id(0);
	const unsigned width = config->width;
	const unsigned height = config->height;

	const unsigned int x = gid % width;
	const int y = gid / width;

	// Check if we have to do something
	if (y >= height)
		return;

	const float epsilon = config->actvateFastRendering ? (config->epsilon * (1.f / 0.75f)) : config->epsilon;
	const uint maxIterations = max(1u,
			config->actvateFastRendering ? (config->maxIterations - 1) : config->maxIterations);

	const float4 mu = (float4)(config->mu[0], config->mu[1], config->mu[2], config->mu[3]);
	const float4 light = (float4)(config->light[0], config->light[1], config->light[2], 0.f);
	const __global CameraCL *camera = &config->camera;

	//--------------------------------------------------------------------------
	// Calculate eye ray
	//--------------------------------------------------------------------------

	const float invWidth = 1.f / width;
	const float invHeight = 1.f / height;
	const float kcx = (x + sampleX) * invWidth - .5f;
	const float4 kcx4 = (float4)kcx;
	const float kcy = (y + sampleY) * invHeight - .5f;
	const float4 kcy4 = (float4)kcy;

	const float4 cameraX = (float4)(camera->x.x, camera->x.y, camera->x.z, 0.f);
	const float4 cameraY = (float4)(camera->y.x, camera->y.y, camera->y.z, 0.f);
	const float4 cameraDir = (float4)(camera->dir.x, camera->dir.y, camera->dir.z, 0.f);
	const float4 cameraOrig = (float4)(camera->orig.x, camera->orig.y, camera->orig.z, 0.f);

	const float4 eyeRayDir =  normalize(cameraX * kcx4 + cameraY * kcy4 + cameraDir);
	const float4 eyeRayOrig = eyeRayDir * (float4)0.1f + cameraOrig;

	//--------------------------------------------------------------------------
	// Check if we hit the bounding sphere
	//--------------------------------------------------------------------------

	float distSet = IntersectBoundingSphere(eyeRayOrig, eyeRayDir);
	float4 hitPoint;
	if (distSet >= 0.f) {
		//--------------------------------------------------------------------------
		// Find the intersection with the set
		//--------------------------------------------------------------------------

		uint steps;
		float4 rayOrig = eyeRayOrig + eyeRayDir * (float4)distSet;
		distSet = IntersectJulia(rayOrig, eyeRayDir, mu, maxIterations,
				epsilon, &hitPoint, &steps);
		if (distSet > epsilon)
			distSet = -1.f;
	}

	//--------------------------------------------------------------------------
	// Check if we hit the floor
	//--------------------------------------------------------------------------

	float distFloor = IntersectFloorSphere(eyeRayOrig, eyeRayDir);

	//--------------------------------------------------------------------------
	// Select the hit point
	//--------------------------------------------------------------------------

	int doShade = 0;
	int useAO = 1;
	float4 diffuse, n, color;
	if ((distSet < 0.f) && (distFloor < 0.f)) {
		// Sky hit
		color = (float4)(0.f, 0.1f, 0.3f, 0.f);
	} else if ((distSet >= 0.f) && ((distFloor < 0.f) || (distSet <= distFloor))) {
		// Set hit
		diffuse = (float4) (1.f, 0.35f, 0.15f, 0.f);
		//diffuse = (float4) (1.f, 0.35f, 0.15f, 0.f);
		//n = NormEstimate(hitPoint, mu, distSet, maxIterations);
		//doShade = 1;
	} else if ((distFloor >= 0.f) && ((distSet < 0.f) || (distFloor <= distSet))) {
		// Floor hit
		hitPoint = eyeRayOrig + eyeRayDir * (float4)distFloor;
		n = hitPoint - WORLD_CENTER;
		n = normalize(n);
		// The most important feature in a ray tracer: a checker texture !
		const int ix = (hitPoint.x > 0.f) ? hitPoint.x : (1.f - hitPoint.x);
		const int iz = (hitPoint.z > 0.f) ? hitPoint.z : (1.f - hitPoint.z);
		if ((ix + iz) % 2)
			diffuse = (float4) (0.75f, 0.75f, 0.75f, 0.f);
		else
			diffuse = (float4) (0.75f, 0.f, 0.f, 0.f);
		doShade = 1;
		useAO = 0;
	}

	//--------------------------------------------------------------------------
	// Select the shadow pass
	//--------------------------------------------------------------------------

	if (doShade) {
		float shadowFactor = 1.f;
		if (config->enableShadow) {
			float4 L = normalize(light -  hitPoint);
			float4 rO = hitPoint + n * 1e-2f;
			float4 shadowHitPoint;

			// Check bounding sphere
			float shadowDistSet = IntersectBoundingSphere(rO, L);
			if (shadowDistSet >= 0.f) {
				uint steps;

				rO = rO + L * (float4)shadowDistSet;
				shadowDistSet = IntersectJulia(rO, L, mu, maxIterations, epsilon,
						&shadowHitPoint, &steps);
				if (shadowDistSet < epsilon) {
					if (useAO) {
						// Use steps count to simulate ambient occlusion
						shadowFactor = 0.6f - min(steps / 255.f, 0.5f);
					} else
						shadowFactor = 0.6f;
				}
			} else
				shadowDistSet = -1.f;
		}

		//--------------------------------------------------------------------------
		// Direct lighting of hit point
		//--------------------------------------------------------------------------

		color = Phong(light, eyeRayOrig, hitPoint, n, diffuse) * shadowFactor;
	}

	//--------------------------------------------------------------------------
	// Write pixel
	//--------------------------------------------------------------------------

	int offset = 3 * (x + y * width);
	
	//float4 outColor = (float4)((x%256)/256.0, (y%256)/256.0 ,1.f, 1.f);
	/*
	float4 outColor = Phong(light, eyeRayOrig, hitPoint, n, diffuse);
  */
  
  /*
  float4 outColor = color;
  	pixels[offset++] = outColor.s0;
		pixels[offset++] = outColor.s1;
		pixels[offset] = outColor.s2;
  */
	color = clamp(color, (float4)(0.f, 0.f ,0.f, 0.f), (float4)(1.f, 1.f ,1.f, 0.f));
	if (enableAccumulation) {
		pixels[offset++] += color.s0;
		pixels[offset++] += color.s1;
		pixels[offset] += color.s2;
	} else {
		pixels[offset++] = color.s0;
		pixels[offset++] = color.s1;
		pixels[offset] = color.s2;
	}
}
