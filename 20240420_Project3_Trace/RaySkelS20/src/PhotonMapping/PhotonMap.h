#ifndef __PHOTONMAP_H__
#define __PHOTONMAP_H__

#include "../vecmath/vecmath.h"
#include "../scene/scene.h"
#include "../scene/light.h"
#include "../scene/ray.h"

#include <random>
#include <algorithm>
#include <vector>
#include <cmath>
#include <limits>
#include <queue>

class EPhoton {
public:
    vec3f position;     
    vec3f direction;
    vec3f power;        // color of light

    EPhoton(const vec3f& position, const vec3f& direction, const vec3f& power)
        : position(position), direction(direction), power(power) {}
    EPhoton(const ray& r, const vec3f& power) : position(r.getPosition()), direction(r.getDirection()), power(power) {}
};

class IPhoton {
public:
    vec3f position;     
    vec3f direction;
    vec3f power;        // color of light

    IPhoton(const vec3f& position, const vec3f& direction, const vec3f& power)
        : position(position), direction(direction), power(power) {}
};

class LightList {
public:
    LightList(Scene* scene);
    void initialize();

    EPhoton* emitPhoton(int nEmitted);
    EPhoton* directionalLightEmit(Light* light, int nEmitted, float prob);
    EPhoton* pointLightEmit(Light* light, int nEmitted, float prob);

private:
    vector<Light*>                          m_directionalLights;
    vector<Light*>                          m_pointLights;
    vector<float>                           m_probability;
    vec3f                                   m_totalPower;
};

class Pair {
public:
    double distance;
    IPhoton* photon;
    
    Pair(double distance, IPhoton* photon)
        : distance(distance), photon(photon) {}

    friend bool operator<(const Pair& a, const Pair& b) { return a.distance > b.distance; }
};

class KDNode {
public:
    IPhoton* photon;
    int axis;
    KDNode* left;
    KDNode* right;

    KDNode(IPhoton* photon, int axis, KDNode* left = nullptr, KDNode* right = nullptr) 
        : photon(photon), axis(axis), left(left), right(right) {}
};

class KDTree {
public:
    KDTree(vector<IPhoton*>& photons, int ndim);
    ~KDTree();

    vector<IPhoton*> knn(vec3f pos, int k, double maxDistance);
    double kernel(vec3f center, vec3f sample, double gatherRadius);


private:
    int     m_ndim;
    KDNode* m_root;

    KDNode* build(vector<IPhoton*>& photons, int start, int end, int depth);
    void clear(KDNode* node);

    void knnRecursive(KDNode* node, const vec3f& pos, int k, double maxDistance, priority_queue<Pair>& pq);
};

#endif // __PHOTONMAP_H__
