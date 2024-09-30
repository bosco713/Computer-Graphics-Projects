// The main ray tracer.

#include <Fl/fl_ask.h>

#include "RayTracer.h"
#include "scene/light.h"
#include "scene/material.h"
#include "scene/ray.h"
#include "fileio/read.h"
#include "fileio/parse.h"
#include "PhotonMapping/PhotonMap.h"

#include <random>
#include <iostream>

// Trace a top-level ray through normalized window coordinates (x,y)
// through the projection plane, and out into the scene.  All we do is
// enter the main ray-tracing method, getting things started by plugging
// in an initial ray weight of (0.0,0.0,0.0) and an initial recursion depth of 0.
vec3f RayTracer::trace( Scene *scene, double x, double y )
{
    ray r( vec3f(0,0,0), vec3f(0,0,0) );
    scene->getCamera()->rayThrough(x, y, r);
	max_depth = scene->getDepth();

	return traceRay(scene, r, thresh, 0, vec3f(1.0, 1.0, 1.0)).clamp();
}

// Do recursive ray tracing!  You'll want to insert a lot of code here
// (or places called from here) to handle reflection, refraction, etc etc.
vec3f RayTracer::traceRay(Scene* scene, const ray& r, const vec3f& thresh, int depth, const vec3f& threshFactor) {
	if (depth > maxDepth)
		return vec3f(0.0, 0.0, 0.0);

	if (threshFactor < thresh)
		return vec3f(0.0, 0.0, 0.0);

	if (m_bCaustics)
		return estimateTotalRadiance(r, 0);

	isect i;

	if (scene->intersect(r, i)) {
		const double& t = i.t;
		vec3f N = i.N;
		const vec3f& Q = r.at(t);
		const Material& m = i.getMaterial();
		const vec3f& d = r.getDirection();
		vec3f I = m.shade(scene, r, i);

		vec3f F = m.kr + (vec3f(1.0, 1.0, 1.0) - m.kr) * fresnel(1.0, m.index, N, -r.getDirection());

		if (!m.kr.iszero()) {
			vec3f R = reflectDirection(N, -d);
			if (!scene->getGlossy()) {
				std::vector<ray> vec_ReflectedRay;
				vec_ReflectedRay.push_back(ray(Q, R));
				for (int i = 0; i < 8; ++i) {
					double num = rand() % 2 - 1;
					double tempX = R[0] * (1 + num);
					num = rand() % 2 - 1;
					double tempY = R[1] * (1 + num);
					num = rand() % 2 - 1;
					double tempZ = R[2] * (1 + num);
					vec3f tempR = vec3f(tempX, tempY, tempZ);
					vec_ReflectedRay.push_back(ray(Q, tempR));
				}
				I += 0.99 * prod(F, traceRay(scene, vec_ReflectedRay[0], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[1], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[2], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[3], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[4], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[5], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[6], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[7], thresh, depth + 1, prod(threshFactor, m.kr)));
				I += 0.01 / 8 * prod(F, traceRay(scene, vec_ReflectedRay[8], thresh, depth + 1, prod(threshFactor, m.kr)));
				// for (ray ReflectedRay : vec_ReflectedRay) {
				// 	I += 1.0 / 5 * prod(F, traceRay(scene, ReflectedRay, thresh, depth + 1, prod(threshFactor, m.kr)));
				// }
			}
			else
			{
				ray ReflectedRay = ray(Q, R);
				I += prod(F, traceRay(scene, ReflectedRay, thresh, depth + 1, prod(threshFactor, m.kr)));
			}
		}

		if (!m.kt.iszero()) {
			double n_i = 0;
			double n_t = 0;
			if (d.dot(N) < 0) {
				n_i = 1.0;		// index of air
				n_t = m.index;
			}
			else {
				n_i = m.index;
				n_t = 1.0;		// index of air
				N = -N;
			}

			double InsideSqrt = insideSqrt(n_i, n_t, N, -d);
			if (n_i <= n_t || InsideSqrt > 0) {
				vec3f T = refractDirectionWithSqrtValue(n_i, n_t, N, -d, InsideSqrt);
				ray RefractedRay = ray(Q, T);
				I += prod(m.kt, traceRay(scene, RefractedRay, thresh, depth + 1, prod(threshFactor, m.kt)));
			}
		}

		return I;
	}
	else {
		return vec3f(0.0, 0.0, 0.0);
	}
}

void RayTracer::buildPhotonMap() {
	if (!m_bSceneLoaded)
		return;
	m_photonMap.clear();
	delete m_KDTree;

	vector<EPhoton*> ephotons;
	LightList lightList(scene);
	for (int i = 0; i < m_nPhotons; i++) {
		ephotons.push_back(lightList.emitPhoton(m_nPhotons));
	}
	vector<IPhoton*> ips;
	for (size_t i = 0; i < ephotons.size(); i++) {
		EPhoton* ep = ephotons[i];
		photonTrace(ep, ips);
		for (size_t i = 0; i < ips.size(); i++) {
			m_photonMap.push_back(ips[i]);
		}
	}
	cout << m_photonMap.size();
	m_KDTree = new KDTree(m_photonMap, 3);
}

void RayTracer::photonTrace(EPhoton* ep, vector<IPhoton*>& ips) {
	ips.clear();
	photonTraceHelper(ep, ips, 0);
}


void RayTracer::photonTraceHelper(EPhoton* ep, vector<IPhoton*>& ips, int bounces) {
	if (bounces > m_maxBounces)
		return;

	isect i;
	ray r(ep->position, ep->direction);

	if (scene->intersect(r, i)) {
		IPhoton* ip = new IPhoton(r.at(i.t), -r.getDirection(), ep->power);
		ips.push_back(ip);

		Material m = i.getMaterial();
		vec3f n = i.N;
		vec3f w_i = -r.getDirection();

		if (!m.kr.iszero() || !m.kt.iszero()) {
			double n_i = 1.0;
			double n_t = m.index;
			double fresnelEffect = fresnel(n_i, n_t, n, w_i);

			if (!m.kr.iszero()) {
				vec3f reflectedDir = m.reflectAbout(w_i, n);
				ray reflectedRay = ray(r.at(i.t), reflectedDir);
				reflectedRay = reflectedRay.bumpedRay(RAY_EPSILON * sign(reflectedDir.dot(i.N)), i.N);
				vec3f newPower = prod(ep->power, m.kr) * fresnelEffect;
				EPhoton* newPhoton = new EPhoton(reflectedRay, newPower);
				photonTraceHelper(newPhoton, ips, bounces + 1);
			}

			if (!m.kt.iszero()) {
				vec3f refracted;
				if (m.refractionDirection(w_i, n, n_i, n_t, refracted)) {
					ray refractedRay(r.at(i.t), refracted);
					refractedRay = refractedRay.bumpedRay(RAY_EPSILON, i.N);
					vec3f newPower = prod(ep->power, m.kt) * (1.0 - fresnelEffect);
					EPhoton* newPhoton = new EPhoton(refractedRay, newPower);
					photonTraceHelper(newPhoton, ips, bounces + 1);
				}
			}
		}

		if (!m.kd.iszero()) {
			vec3f diffuseDir = cosHemiRandom(n);
			ray diffuseRay = ray(r.at(i.t), diffuseDir);
			diffuseRay = diffuseRay.bumpedRay(RAY_EPSILON, i.N);
			vec3f newPower = prod(ep->power, m.kd);
			EPhoton* newPhoton = new EPhoton(diffuseRay, newPower);
			photonTraceHelper(newPhoton, ips, bounces + 1);
		}
	}
}

vec3f RayTracer::estimateTotalRadiance(const ray& r, int depth) {
	vec3f L(0.0, 0.0, 0.0);
	if (m_emit) L += estimateEmittedLight(r);
		
	L += estimateTotalScatteredRadiance(r, depth);

	return L;
}

vec3f RayTracer::estimateEmittedLight(ray r) {
	vec3f L(0.0, 0.0, 0.0);
	isect i;

	if (scene->intersect(r, i))
		L += i.getMaterial().ke;

	return L;
}

vec3f RayTracer::estimateTotalScatteredRadiance(const ray& r, int depth) {
	vec3f L(0.0, 0.0, 0.0);
	isect i;

	if (scene->intersect(r, i)) {
		L += estimateReflectedDirectLight(r, i, depth);
		if (m_IImp || depth > 0) L += estimateImpulseScatteredIndirectLight(r, i, depth + 1);
		if (m_IDiff || depth > 0) L += estimateDiffuselyReflectedIndirectLight(r, i);
	}

	return L;
}

vec3f RayTracer::estimateReflectedDirectLight(const ray& r, const isect& i, int depth) {
	vec3f L(0.0, 0.0, 0.0);
	if (m_DImp || depth > 0) L += estimateImpulseScatteredDirectLight(r, i);
	if (m_DDiff || depth > 0) L += estimateDiffuselyReflectedDirectLight(r, i);
	
	return L;
}

vec3f RayTracer::estimateImpulseScatteredDirectLight(const ray& r, const isect& i) {
	vec3f L(0.0, 0.0, 0.0);
	vector<Material::Impulse*> impulseArray;

	i.getMaterial().getBSDFImpulses(-r.getDirection(), i, impulseArray);
	for (size_t n = 0; n < impulseArray.size(); n++) {
		isect i2;

		ray r(r.at(i.t), impulseArray[n]->direction);
		r = r.bumpedRay(RAY_EPSILON * sign(r.getDirection().dot(i.N)), i.N);

		if (scene->intersect(r, i2))
			L += prod(impulseArray[n]->magnitude, i2.getMaterial().ke);
	}

	
	return L;
}

vec3f RayTracer::estimateImpulseScatteredIndirectLight(const ray& r, const isect& i, int depth) {
	vec3f L(0.0, 0.0, 0.0);

	if (depth > m_maxBounces)
		return L;

	vector<Material::Impulse*> impulseArray;

	i.getMaterial().getBSDFImpulses(-r.getDirection(), i, impulseArray);
	for (size_t n = 0; n < impulseArray.size(); n++) {
		ray r(r.at(i.t), impulseArray[n]->direction);
		r = r.bumpedRay(RAY_EPSILON * sign(r.getDirection().dot(i.N)), i.N);
		L += prod(impulseArray[n]->magnitude, estimateTotalScatteredRadiance(r, depth + 1));
	}

	return L;
}

vec3f RayTracer::estimateDiffuselyReflectedDirectLight(const ray& r, const isect& i) {
	vec3f L(0.0, 0.0, 0.0);
	
	for (Scene::cliter li = scene->beginLights(); li != scene->endLights(); ++li) {
		Light* light = *li;
		for (int n = 0; n < m_intensity; n ++ ){
			if (scene->lineOfSight(
				r.at(i.t) + i.N * NORMAL_EPSILON,
				light->getPostion(r.at(i.t)))) {
				vec3f w_i = light->getPostion(r.at(i.t)) - r.at(i.t);
				const double distance2 = w_i.length_squared();
				w_i /= sqrt(distance2);

				const vec3f& E_i = light->getColor(r.at(i.t));

				L += prod(i.getMaterial().evaluateBSDF(w_i, -r.getDirection(), i), E_i) * max(0.0, w_i.dot(i.N));
			}
		}
	}
	
	return L;
}

vec3f RayTracer::estimateDiffuselyReflectedIndirectLight(const ray& r, const isect& i) {
	vec3f L(0.0, 0.0, 0.0);

	vector<IPhoton*> photonArray = m_KDTree->knn(r.at(i.t), k, maxDistance);
	
	for (size_t n = 0; n < photonArray.size(); n++)
		L += prod(photonArray[n]->power * m_KDTree->kernel(photonArray[n]->position, r.at(i.t), maxDistance), i.getMaterial().evaluateBSDF(photonArray[n]->direction, -r.getDirection(), i));
	
	
	return L;
}



RayTracer::RayTracer()
{
	buffer = NULL;
	buffer_width = buffer_height = 256;
	scene = NULL;

	m_bSceneLoaded = false;
}


RayTracer::~RayTracer()
{
	delete [] buffer;
	delete scene;
}

void RayTracer::getBuffer( unsigned char *&buf, int &w, int &h )
{
	buf = buffer;
	w = buffer_width;
	h = buffer_height;
}

double RayTracer::aspectRatio()
{
	return scene ? scene->getCamera()->getAspectRatio() : 1;
}

bool RayTracer::sceneLoaded()
{
	return m_bSceneLoaded;
}

bool RayTracer::loadScene( char* fn )
{
	try
	{
		scene = readScene( fn );
	}
	catch( ParseError pe )
	{
		fl_alert( "ParseError: %s\n", pe );
		return false;
	}

	if( !scene )
		return false;
	
	buffer_width = 256;
	buffer_height = (int)(buffer_width / scene->getCamera()->getAspectRatio() + 0.5);

	bufferSize = buffer_width * buffer_height * 3;
	buffer = new unsigned char[ bufferSize ];
	
	// separate objects into bounded and unbounded
	scene->initScene();
	
	// Add any specialized scene loading code here
	
	m_bSceneLoaded = true;

	return true;
}

void RayTracer::traceSetup( int w, int h, int maxDepth, const vec3f& thresh )
{
	if( buffer_width != w || buffer_height != h )
	{
		buffer_width = w;
		buffer_height = h;

		bufferSize = buffer_width * buffer_height * 3;
		delete [] buffer;
		buffer = new unsigned char[ bufferSize ];
	}
	memset( buffer, 0, w*h*3 );

	this->maxDepth = maxDepth;
	this->thresh = thresh;
}

void RayTracer::traceLines( int start, int stop )
{
	vec3f col;
	if( !scene )
		return;

	if( stop > buffer_height )
		stop = buffer_height;

	for( int j = start; j < stop; ++j )
		for( int i = 0; i < buffer_width; ++i )
				tracePixel(i, j);
}

void RayTracer::tracePixel( int i, int j )
{
	vec3f col;

	if( !scene )
		return;

	double x = double(i)/double(buffer_width);
	double y = double(j)/double(buffer_height);

	col = trace( scene,x,y );

	unsigned char *pixel = buffer + ( i + j * buffer_width ) * 3;

	pixel[0] = (int)( 255.0 * col[0]);
	pixel[1] = (int)( 255.0 * col[1]);
	pixel[2] = (int)( 255.0 * col[2]);
}

/*
vec3f RayTracer::reflectDirection(const vec3f& N, const vec3f& n_d) {
	return n_d - 2 * n_d.dot(N) * N;
}

bool RayTracer::refractDirection(float n_i, float n_t, const vec3f& N, const vec3f& d, vec3f& T) {
	float eta = n_i / n_t;
	float cos_i = -N.dot(d);
	float sin_t2 = eta * eta * (1.0 - cos_i * cos_i);
	if (sin_t2 > 1.0) return false;  // Total internal reflection
	float cos_t = sqrt(1.0 - sin_t2);
	T = eta * d + (eta * cos_i - cos_t) * N;
	return true;
}
*/

float RayTracer::fresnel(float n_i, float n_t, const vec3f& N, const vec3f& d) {
	if (N.dot(d) < 0) swap(n_i, n_t);
	float r0 = (n_i - n_t) / (n_i + n_t);
	r0 = r0 * r0;
	float cos_t = abs(N.dot(d));
	if (n_i > n_t) {
		float eta = n_i / n_t;
		float sin_t2 = eta * eta * (1.0 - cos_t * cos_t);
		if (sin_t2 > 1.0) return 1.0; // Total internal reflection
		cos_t = sqrt(1.0 - sin_t2);
	}
	return r0 + (1.0 - r0) * pow(1.0 - cos_t, 5.0);
}

// return normalized reflection vector shotting away from the surface
vec3f RayTracer:: reflectDirection(const vec3f& n, const vec3f& neg_d) {
	return (2 * (neg_d.dot(n)) * n - neg_d).normalize();
}

double RayTracer:: insideSqrt(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d) {
	double n_r = n_i / n_t;
	return (1 - (n_r * n_r) * (1 - pow(n.dot(neg_d), 2.0)));
}

bool RayTracer:: TIR(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d) {
	if (n_i > n_t) {
		double n_r = n_i / n_t;
		if ((1 - (n_r * n_r) * (1 - pow(n.dot(neg_d), 2.0)) <= 0)) {
			return true;
		}
	}
	return false;
}

vec3f RayTracer::refractDirection(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d) {
	double n_r = n_i / n_t;
	return ((n_r * (n.dot(neg_d)) - sqrt(1 - (n_r * n_r) * (1 - pow(n.dot(neg_d), 2.0)))) * n - n_r * neg_d).normalize();
}

vec3f RayTracer::refractDirectionWithSqrtValue(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d, const double& InsideSqrt) {
	double n_r = n_i / n_t;
	return ((n_r * (n.dot(neg_d)) - sqrt(InsideSqrt)) * n - n_r * neg_d).normalize();
}
