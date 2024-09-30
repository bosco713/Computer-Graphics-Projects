#include "ray.h"
#include "material.h"
#include "light.h"

// Apply the phong model to this point on the surface of the object, returning
// the color of that point.
vec3f Material::shade( Scene *scene, const ray& r, const isect& i ) const
{
	// YOUR CODE HERE

	// For now, this method just returns the diffuse color of the object.
	// This gives a single matte color for every distinct surface in the
	// scene, and that's it.  Simple, but enough to get you started.
	// (It's also inconsistent with the phong model...)

	// Your mission is to fill in this method with the rest of the phong
	// shading model, including the contributions of all the light sources.
    // You will need to call both distanceAttenuation() and shadowAttenuation()
    // somewhere in your code in order to compute shadows and light falloff.

	vec3f Q = r.at(i.t);
	vec3f transparency = vec3f(1.0, 1.0, 1.0) - kt;

	vec3f I = ke;
	vec3f AmbientLightColor = scene->getAmbientLightColor();
	AmbientLightColor *= scene->getAmbientLight();
	vec3f ambient = prod(ka, AmbientLightColor);
	I += prod(transparency, ambient);

	for (Scene::cliter li = scene->beginLights(); li != scene->endLights(); li++) {
		Light* light = *li;
		vec3f L = light->getDirection(Q);
		vec3f N = i.N.normalize();
		/*
		if (N.dot(L) < 0) {
			N = -N;
		}
		*/
		vec3f R = (2 * L.dot(N) * N - L).normalize();
		vec3f V = -r.getDirection();

		double distAtten = light->distanceAttenuation(Q);
		vec3f shadowAtten = light->shadowAttenuation(Q);

		vec3f diffuse = prod(kd * max(L.dot(N), 0.0), transparency);
		vec3f specular = ks * pow(max(R.dot(V), 0.0), shininess * 256.0);

		vec3f coef = prod(diffuse + specular, light->getColor(Q));

		I += prod(coef, distAtten * shadowAtten);
	}

	return I;
}

vec3f Material::evaluateBSDF(vec3f& w_i, vec3f& w_o, const isect& i) const {
	vec3f n = i.N.normalize();
	vec3f result(0.0, 0.0, 0.0);

	if (n.dot(w_i) * n.dot(w_o) > 0) {
		vec3f diffuse = kd * (1.0 / PI);

		vec3f r = (2 * (w_i.dot(n)) * n - w_i).normalize();
		vec3f specular = ks * pow(max(0.0, r.dot(w_o)), shininess);

		result += (diffuse + specular) * max(0.0, n.dot(w_o));
	}

	if (!kt.iszero()) {
		double n_i = 0;
		double n_t = 0;
		if (w_i.dot(n) > 0) {
			n_i = 1.0;
			n_t = index;
		}
		else {
			n_i = index;
			n_t = 1.0;
			n = -n;
		}

		vec3f tDirection;
		if (refractionDirection(w_i, n, n_i, n_t, tDirection) && tDirection.dot(w_o) > 0) {
			double cos_i = abs(n.dot(w_i));
			double cos_t = abs(n.dot(tDirection));
			double F_r = computeReflectance(cos_i, shininess);

			vec3f transmission = kt * (1.0 - F_r) * (cos_t / cos_i);
			result += transmission * max(0.0, tDirection.dot(w_o));
		}
	}

	return result;
}





void Material::getBSDFImpulses(vec3f& w_i, const isect& i, vector<Impulse*>& impulseArray) const {
	vec3f n = i.N;
	vec3f p_L = kd;							// Lambertian Reflectivity
	vec3f F_0 = ks;							// Glossy Reflection Coefficient
	double smoothness = shininess;

	const double cos_i = max(NORMAL_EPSILON, w_i.dot(n));
	vec3f F = !F_0.iszero() ? computeF(F_0, cos_i, smoothness) : vec3f(0.0, 0.0, 0.0);		

	if ((smoothness == 1.0) && !F_0.iszero()) {
		Impulse* imp = new Impulse(reflectAbout(w_i, n), F);
		impulseArray.push_back(imp);
	}

	const vec3f& tMagnitude = prod(kt, prod((vec3f(1.0, 1.0, 1.0) - F), (vec3f(1.0, 1.0, 1.0) - prod((vec3f(1.0, 1.0, 1.0) - F), p_L))));

	if (!tMagnitude.iszero()) {
		double n_i = 0;
		double n_t = 0;
		if (w_i.dot(n) > 0) {
			n_i = 1.0;
			n_t = index;
		}
		else {
			n_i = index;
			n_t = 1.0;
			n = -n;
		}
		vec3f tDirection;
		bool validTransimission = refractionDirection(w_i, n, n_i, n_t, tDirection);
		if (!tDirection.iszero()) {
			Impulse* imp = new Impulse(tDirection, tMagnitude);
			impulseArray.push_back(imp);
		}
	}
}

double Material::computeReflectance(double cos_i, double shininess) const {
	double r0 = pow((1.0 - shininess) / (1.0 + shininess), 2);
	return r0 + (1.0 - r0) * pow(1.0 - cos_i, 5);
}

double Material::computeTransmittance(double cos_i, double index) const {
	double sin_i = sqrt(1.0 - cos_i * cos_i);
	double sin_t = sin_i / index;
	if (sin_t >= 1.0) return 0; // Total internal reflection

	double cos_t = sqrt(1.0 - sin_t * sin_t);
	double rs = pow((index * cos_i - cos_t) / (index * cos_i + cos_t), 2);
	double rp = pow((cos_i - index * cos_t) / (cos_i + index * cos_t), 2);
	return 0.5 * (rs + rp);
}

vec3f Material::reflectAbout(const vec3f& w_i, const vec3f& n) const {
	vec3f normalized_n = n.normalize();
	return w_i - 2.0 * w_i.dot(normalized_n) * normalized_n;
}

bool Material::refractionDirection(const vec3f& w_i, const vec3f& n, double n_i, double n_t, vec3f& refracted) const {
	vec3f normalized_n = n.normalize();
	double cos_i = -w_i.dot(normalized_n);
	double sin_i_squared = 1.0 - cos_i * cos_i;
	double n_ratio = n_i / n_t;
	double sin_t_squared = n_ratio * n_ratio * sin_i_squared;

	if (sin_t_squared > 1.0) {
		// Total internal reflection
		return false;
	}

	double cos_t = sqrt(1.0 - sin_t_squared);
	refracted = n_ratio * w_i + (n_ratio * cos_i - cos_t) * normalized_n;
	return true;
}


vec3f Material::computeF(const vec3f& F0, double cos_i, double smoothness) const {
	return (F0[0] + F0[1] + F0[2] > NORMAL_EPSILON) ? lerp(F0, vec3f(1.0, 1.0, 1.0), 0.9f * pow(smoothness * smoothness * (1.0f - max(0.0, cos_i)), 5)) : F0;
}

double Material::glossyScatter(const vec3f& w_i, const vec3f& w_o, const vec3f& n, double smoothness) const {
	w_i.normalize();
	w_o.normalize();
	n.normalize();

	vec3f r = 2 * n.dot(w_i) * n - w_i;

	float dot_product = r.dot(w_o);

	if (dot_product < 0) {
		dot_product = 0;
	}

	float intensity = pow(dot_product, smoothness);

	return intensity;
}
















