#include <cmath>

#include "light.h"
#include "../PhotonMapping/PhotonMap.h"

double DirectionalLight::distanceAttenuation( const vec3f& P ) const
{
	// distance to light is infinite, so f(di) goes to 0.  Return 1.
	return 1.0;
}


vec3f DirectionalLight::shadowAttenuation( const vec3f& P ) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f d = getDirection(P);
	ray shadowRay(P, d);
	isect i;
	if (scene->intersect(shadowRay, i)) {
		return i.getMaterial().kt;
	}
	return vec3f(1.0, 1.0, 1.0);
}

vec3f DirectionalLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f DirectionalLight::getDirection( const vec3f& P ) const
{
	return -orientation;
}

double PointLight::distanceAttenuation( const vec3f& P ) const
{
	// YOUR CODE HERE

	// You'll need to modify this method to attenuate the intensity 
	// of the light based on the distance between the source and the 
	// point P.  For now, I assume no attenuation and just return 1.0
	double d = (position - P).length();
	double constant = 1 * (scene->getConstantAttenuation()) * constant_attenuation;
	double linear = 20 * (scene->getLinearAttenuation()) * linear_attenuation;
	double quadratic = 400 * (scene->getQuadraticAttenuation()) * quadratic_attenuation;
	double base = constant + linear * d + quadratic * d * d + RAY_EPSILON;
	double attenuation = min(1.0, 1.0 / base);
	return attenuation;
}

vec3f PointLight::getColor( const vec3f& P ) const
{
	// Color doesn't depend on P 
	return color;
}

vec3f PointLight::getDirection( const vec3f& P ) const
{
	return (position - P).normalize();
}


vec3f PointLight::shadowAttenuation(const vec3f& P) const
{
    // YOUR CODE HERE:
    // You should implement shadow-handling code here.
	vec3f d = getDirection(P);
	ray shadowRay(P, d);
	isect i;
	if (scene->intersect(shadowRay, i)) {
		return i.getMaterial().kt;
	}
	return vec3f(1.0, 1.0, 1.0);
}

SpotLight:: SpotLight(Scene* scene, const vec3f& pos, const vec3f& color 
	, const double& const_att, const double& linear_att, const double& quad_att
	, const vec3f& dir, const double& cutoff)
	: PointLight(scene, pos, color, const_att, linear_att, quad_att), direction(dir)
	, cosCutoffAngle(cos(cutoff / 180 * acos(0)*2)) { }

double SpotLight::distanceAttenuation(const vec3f& P) const
{
	double spotFactor = 1.0;
	vec3f L = (position - P).normalize();
	double cosAngle = L.dot(direction.normalize());
	cosAngle = cosAngle < 0 ? -cosAngle : cosAngle;
	if (cosAngle < cosCutoffAngle) {
		spotFactor = 0;
	}
	return spotFactor * PointLight::distanceAttenuation(P);
}

WarnLight::WarnLight(Scene* scene, const vec3f& pos, const vec3f& color
	, const double& const_att, const double& linear_att, const double& quad_att
	, const vec3f& dir)
	: PointLight(scene, pos, color, const_att, linear_att, quad_att), direction(dir), exponent(64.0) { }

double WarnLight::distanceAttenuation(const vec3f& P) const
{
	double spotFactor = 1.0;
	vec3f L = (position - P).normalize();
	double cosAngle = L.dot(direction.normalize());
	cosAngle = cosAngle < 0 ? -cosAngle : cosAngle;
	spotFactor = pow(cosAngle, exponent);
	return spotFactor;
}

ShapeLight::ShapeLight(Scene* scene, const vec3f& pos, const vec3f& color
	, const double& const_att, const double& linear_att, const double& quad_att
	, const vec3f& dir, const vec3f& para, const vec3f& up, const vec3f& shape)
	: PointLight(scene, pos, color, const_att, linear_att, quad_att), direction(dir)
	, parameter(para), upVector(up), shape(shape) {
	vec3f zAxis = (direction-position).normalize();
	vec3f xAxis = zAxis.cross(up).normalize();
	vec3f yAxis = xAxis.cross(zAxis).normalize();
	transformMatrix = { { xAxis[0], xAxis[1], xAxis[2], 0 },
		{ yAxis[0], yAxis[1], yAxis[2], 0 },
		{ zAxis[0], zAxis[1], zAxis[2], 0 },
		{ 0, 0, 0, 1 } };
}

double ShapeLight::distanceAttenuation(const vec3f& P) const
{
	double spotFactor = 0.0;
	vec4f transformedCoordinate = transformMatrix * vec4f({ P[0],P[1],P[2],1 });
	double transformedX = transformedCoordinate[0];
	double transformedZ = transformedCoordinate[1];
	double size = parameter[0];
	switch (int(shape[0])) {
	case 0:	// triangle
		if (transformedZ > -size / 2 && transformedZ < sqrt(3) * transformedX + size && transformedZ < -sqrt(3) * transformedX + size) {
			spotFactor = 1.0;
		}
		break;
	case 1:	// square
		if (transformedX > -size && transformedX<size && transformedZ>-size && transformedZ < size) {
			spotFactor = 1.0;
		}
		break;
	case 2:	// star
		if (transformedX > -size / 2 && transformedX < sqrt(3) * transformedZ + size && transformedX < -sqrt(3) * transformedZ + size) {
			spotFactor = 1.0;
		}
		if (transformedX < size / 2 && transformedX > sqrt(3) * transformedZ - size && transformedX > -sqrt(3) * transformedZ - size) {
			spotFactor = 1.0;
		}
		break;
	default:
		printf("We don't support this type, we will provide you a point light\n");
		spotFactor = 1.0;
	}
	return spotFactor * PointLight::distanceAttenuation(P);
}