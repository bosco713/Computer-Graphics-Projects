#include "particleSystem.h"

#include <limits.h>
#include <FL/gl.h>
#include "modelerdraw.h"
#include <functional>

void Particle::draw() {
	setDiffuseColor(0.5, 0.5, 0.5);

	glEnable(GL_POINT_SMOOTH);
	glPointSize(8);

	glPushMatrix();
	glBegin(GL_POINTS);
	glVertex3f(position[0], position[1], position[2]);
	glEnd();
	glPopMatrix();

	glPointSize(1);
	glDisable(GL_POINT_SMOOTH);
}

/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() {
	std::function<void(Particle&)> gravity = [](Particle& p) {
		Vec3f g(0, -9.8, 0);
		p.netForce += g * p.mass;
		};

	std::function<void(Particle&)> viscousDrag = [](Particle& p) {
		double K = 0.1;
		p.netForce -= K * p.velocity;
		};
	particles.reserve(100);
}

/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem()
{
	// TODO
	particles.clear();
}


/******************
 * Simulation fxns
 ******************/

 /** Start the simulation */
void ParticleSystem::startSimulation(float t)
{

	// TODO

	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{

	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{

	// TODO

	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	bake_fps = t - simulationTime;
	simulationTime = t;
	if (simulate) {
		if (bakeStates.find(t) == bakeStates.end()) {
			for (std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
			{
				it->nextPos(bake_fps);
			}
			bakeParticles(t);
		}
		else
		{
			particles = bakeStates[t];
		}
	}
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{

	// TODO
	if (simulate) {
		for (std::vector<Particle>::iterator it = particles.begin(); it != particles.end(); it++) {
			it->draw();
		}
	}
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t)
{

	// TODO
	bakeStates.insert(std::pair<float, std::vector<Particle>>(t, particles));
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
	bakeStates.clear();
}

void ParticleSystem::spawnParticles(Vec3f pos, int num) {
	std::mt19937 gen(std::random_device{}());
	std::uniform_real_distribution<> distMass(0.2, 5.0);
	std::uniform_real_distribution<> distVelocity(-10.0, 10.0);

	std::function<void(Particle&)> gravity = [](Particle& p) {
		Vec3f g(0, -9.8, 0);
		p.netForce += g * p.mass;
		};

	for (int i = 0; i < num; i++) {
		double mass = distMass(gen);
		Vec3f initialVelocity(distVelocity(gen), distVelocity(gen), distVelocity(gen));
		Particle p(pos, mass);
		p.velocity = initialVelocity;

		p.addForce(gravity);

		particles.push_back(p);
	}
}

