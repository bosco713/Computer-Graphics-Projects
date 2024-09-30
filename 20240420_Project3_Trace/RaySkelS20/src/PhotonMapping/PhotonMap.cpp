#include "PhotonMap.h"

LightList::LightList(Scene* scene) {
    for (Scene::cliter li = scene->beginLights(); li != scene->endLights(); li++) {
        Light* light = *li;
        if (dynamic_cast<DirectionalLight*>(light)) {
                m_directionalLights.push_back(light);
        } 
        else if (dynamic_cast<PointLight*>(light)) {
            m_pointLights.push_back(light);
        }
    }

    initialize();
}

void LightList::initialize() {
    for (size_t i = 0; i < m_directionalLights.size(); i++) 
        m_totalPower += m_directionalLights[i]->getColor({});
    for (size_t i = 0; i < m_pointLights.size(); i++) 
        m_totalPower += m_pointLights[i]->getColor({});
    
    for (size_t i = 0; i < m_directionalLights.size(); i++) 
        m_probability.push_back(m_directionalLights[i]->getColor({}).average() / m_totalPower.average());
    for (size_t i = 0; i < m_pointLights.size(); i++) 
        m_probability.push_back(m_pointLights[i]->getColor({}).average() / m_totalPower.average());
}

EPhoton* LightList::emitPhoton(int nEmitted) {
    int selected = 0;
    double u = getRandomReal();
    
    for (int i = 0; i < m_probability.size(); i++) {
        u -= m_probability[i];
        
        if (u < 0.0) {
            selected = i;
            break;
        }
    }

    if (selected < m_directionalLights.size())
        return directionalLightEmit(m_directionalLights[selected], nEmitted, m_probability[selected]);
    else
        return pointLightEmit(m_pointLights[selected - m_directionalLights.size()], nEmitted, m_probability[selected]);
}

EPhoton* LightList::directionalLightEmit(Light* light, int nEmitted, float prob) {
    vec3f position = light->getPostion(vec3f());
    vec3f direction = light->getDirection(vec3f());
    vec3f power = light->getColor(vec3f()) / (nEmitted * prob);
    EPhoton* photon = new EPhoton(position, direction, power);
    return photon;
}

EPhoton* LightList::pointLightEmit(Light* light, int nEmitted, float prob) {
    vec3f position = light->getPostion(vec3f());
    vec3f direction = uniformSampleSphere().normalize();
    vec3f power = light->getColor(vec3f()) /  (nEmitted * prob);
    EPhoton* photon = new EPhoton(position, direction, power);
    return photon;
}

KDTree::KDTree(vector<IPhoton*>& photons, int ndim) : m_ndim(ndim), m_root(nullptr) {
    if (!photons.empty()) {
        m_root = build(photons, 0, photons.size(), 0);
    }
}

KDTree::~KDTree() {
    clear(m_root);
}

KDNode* KDTree::build(vector<IPhoton*>& photons, int start, int end, int depth) {
    if (start >= end) return nullptr;

    int axis = depth % m_ndim;
    int mid = start + (end - start) / 2;

    nth_element(photons.begin() + start, photons.begin() + mid, photons.begin() + end, [axis](const IPhoton* a, const IPhoton* b) {
        return a->position[axis] < b->position[axis];
        });

    KDNode* node = new KDNode(photons[mid], axis);
    node->left = build(photons, start, mid, depth + 1);
    node->right = build(photons, mid + 1, end, depth + 1);

    return node;
}

void KDTree::clear(KDNode* node) {
    if (node != nullptr) {
        clear(node->left);
        clear(node->right);
        delete node;
    }
}

vector<IPhoton*> KDTree::knn(vec3f pos, int k, double maxDistance) {
    vector<IPhoton*> result;
    priority_queue<Pair> pq;

    knnRecursive(m_root, pos, k, maxDistance, pq);

    while (!pq.empty()) {
        result.push_back(pq.top().photon);
        pq.pop();
    }

    reverse(result.begin(), result.end());
    return result;
}

void KDTree::knnRecursive(KDNode* node, const vec3f& pos, int k, double maxDistance, priority_queue<Pair>& pq) {
    if (node == nullptr) return;

    vec3f diff = node->photon->position - pos;
    double distance = diff.length();

    if (distance < maxDistance) {
        if (pq.size() < k) {
            pq.emplace(distance, node->photon);
        }
        else if (distance < pq.top().distance) {
            pq.pop();
            pq.emplace(distance, node->photon);
        }
    }

    int axis = node->axis;
    double delta = pos[axis] - node->photon->position[axis];

    KDNode* nearNode = delta < 0 ? node->left : node->right;
    KDNode* farNode = delta >= 0 ? node->left : node->right;

    knnRecursive(nearNode, pos, k, maxDistance, pq);

    if (fabs(delta) < maxDistance) {
        knnRecursive(farNode, pos, k, maxDistance, pq);
    }
}

double KDTree::kernel(vec3f center, vec3f sample, double gatherRadius) {
    double r = (sample - center).length();
    double x = r / gatherRadius;
    double h = max(1.0 - x, 0.0);

    return h * (3.0 / (PI * gatherRadius * gatherRadius));
}

