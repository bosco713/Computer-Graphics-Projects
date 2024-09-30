#ifndef __PARTICLE_SYSTEM_H__
#define __PARTICLE_SYSTEM_H__

#include "vec.h"
#include <vector>
#include <map>
#include <random>
#include <functional>

class Particle {
public:
    double mass;
    Vec3f position;
    Vec3f velocity;
    Vec3f netForce;
    std::vector<std::function<void(Particle&)>> forces;

    Particle(Vec3f p, double m) : position(p), mass(m), velocity(Vec3f(0, 0, 0)), netForce(Vec3f(0, 0, 0)) {}

    void addForce(const std::function<void(Particle&)>& f) {
        forces.push_back(f);
    }

    void applyForces() {
        for (auto& f : forces) {
            f(*this);
        }
    }

    void nextPos(float dt) {
        applyForces();
        velocity += netForce / mass * dt;
        position += velocity * dt;
    }

    void draw();
};

class ParticleSystem {
public:
    ParticleSystem();
    virtual ~ParticleSystem();
    virtual void drawParticles(float t);
    virtual void bakeParticles(float t);
    virtual void computeForcesAndUpdateParticles(float t);
    virtual void resetSimulation(float t);
    virtual void startSimulation(float t);
    virtual void stopSimulation(float t);
    virtual void clearBaked();
    void spawnParticles(Vec3f pos, int num);

    // These accessor fxns are implemented for you
    float getBakeStartTime() { return bake_start_time; }
    float getBakeEndTime() { return bake_end_time; }
    float getBakeFps() { return bake_fps; }
    bool isSimulate() { return simulate; }
    bool isDirty() { return dirty; }
    void setDirty(bool d) { dirty = d; }

protected:
    float simulationTime;
    std::vector<Particle> particles;
    std::map<float, std::vector<Particle>> bakeStates;

    float bake_fps;
    float bake_start_time;
    float bake_end_time;

    bool simulate;
    bool dirty;
};

#endif // __PARTICLE_SYSTEM_H__
