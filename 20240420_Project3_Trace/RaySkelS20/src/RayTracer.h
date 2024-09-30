#ifndef __RAYTRACER_H__
#define __RAYTRACER_H__

// The main ray tracer.

#include "scene/scene.h"
#include "scene/ray.h"
#include "PhotonMapping/PhotonMap.h"

class RayTracer
{
public:
	RayTracer();
	~RayTracer();

	vec3f trace(Scene* scene, double x, double y);
	vec3f traceRay(Scene* scene, const ray& r, const vec3f& thresh, int depth, const vec3f& threshFactor);

	void buildPhotonMap();
	void photonTrace(EPhoton* ep, vector<IPhoton*>& ips);
	void photonTraceHelper(EPhoton* ep, vector<IPhoton*>& ips, int bounces);
	vec3f estimateTotalRadiance(const ray& r, int depth);
	vec3f estimateEmittedLight(ray r);
	vec3f estimateTotalScatteredRadiance(const ray& r, int depth);
	vec3f estimateReflectedDirectLight(const ray& r, const isect& i, int depth);
	vec3f estimateImpulseScatteredDirectLight(const ray& r, const isect& i);
	vec3f estimateImpulseScatteredIndirectLight(const ray& r, const isect& i, int depth);
	vec3f estimateDiffuselyReflectedDirectLight(const ray& r, const isect& i);
	vec3f estimateDiffuselyReflectedIndirectLight(const ray& r, const isect& i);

	void getBuffer(unsigned char*& buf, int& w, int& h);
	double aspectRatio();
	void traceSetup(int w, int h, int maxDepth, const vec3f& thresh);
	void traceLines(int start = 0, int stop = 10000000);
	void tracePixel(int i, int j);

	bool loadScene(char* fn);

	bool sceneLoaded();

	/*
	vec3f reflectDirection(const vec3f& n, const vec3f& n_d);
	bool RayTracer::refractDirection(double n_i, double n_t, const vec3f& N, const vec3f& d, vec3f& T);
	*/
	float fresnel(float n_i, float n_t, const vec3f& N, const vec3f& d);
	vec3f reflectDirection(const vec3f& n, const vec3f& neg_d);
	double insideSqrt(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d);
	bool TIR(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d);
	vec3f refractDirection(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d);
	vec3f refractDirectionWithSqrtValue(const double& n_i, const double& n_t, const vec3f& n, const vec3f& neg_d, const double& InsideSqrt);

	
	int m_maxBounces{ 30 };
	bool m_bCaustics{ false };
	int m_nPhotons{ 50000 };
	int k{ 20 };
	int m_intensity{ 1 };

	bool m_emit{ true };
	bool m_IImp{ true };
	bool m_IDiff{ true };
	bool m_DDiff{ true };
	bool m_DImp{ true };

private:
	unsigned char* buffer;
	int buffer_width, buffer_height;
	int bufferSize;
	Scene* scene;
	int max_depth = 0;
	int maxDepth;
	bool m_bSceneLoaded;
	vec3f thresh;

	KDTree* m_KDTree{ nullptr };
	vector<IPhoton*> m_photonMap;
	double maxDistance{ 0.01 };
};

#endif // __RAYTRACER_H__
