#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "scene.h"

class Light
	: public SceneElement
{
public:
	virtual vec3f shadowAttenuation(const vec3f& P) const = 0;
	virtual double distanceAttenuation( const vec3f& P ) const = 0;
	virtual vec3f getColor( const vec3f& P ) const = 0;
	virtual vec3f getDirection( const vec3f& P ) const = 0;
	virtual vec3f getPostion( const vec3f& P ) const = 0;

protected:
	Light( Scene *scene, const vec3f& col )
		: SceneElement( scene ), color( col ) {}

	vec3f 		color;
};

class DirectionalLight
	: public Light
{
public:
	DirectionalLight(Scene* scene, const vec3f& orien, const vec3f& color)
		: Light(scene, color), orientation(orien) {
		position = orientation * 1e5;
	}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	virtual vec3f getPostion( const vec3f& P ) const override { return position; };

protected:
	vec3f 		orientation;
	vec3f		position;
};

class PointLight
	: public Light
{
public:
	PointLight(Scene* scene, const vec3f& pos, const vec3f& color
		, const double& const_att, const double& linear_att, const double& quad_att)
		: Light(scene, color), position(pos)
		, constant_attenuation(const_att), linear_attenuation(linear_att), quadratic_attenuation(quad_att) {
	}
	virtual vec3f shadowAttenuation(const vec3f& P) const;
	virtual double distanceAttenuation( const vec3f& P ) const;
	virtual vec3f getColor( const vec3f& P ) const;
	virtual vec3f getDirection( const vec3f& P ) const;
	virtual vec3f getPostion( const vec3f& P ) const override { return position; };

protected:
	vec3f position;
	double constant_attenuation;
	double linear_attenuation;
	double quadratic_attenuation;
};


class AmbientLight
	: public Light
{
public:
	AmbientLight(Scene* scene, const vec3f& color) 
		: Light(scene, color){}
	virtual vec3f shadowAttenuation(const vec3f& P) const { return vec3f(1.0, 1.0, 1.0); };
	virtual double distanceAttenuation(const vec3f& P) const { return 1.0; };
	virtual vec3f getColor(const vec3f& P) const { return color; };
	virtual vec3f getDirection(const vec3f& P) const { return vec3f(1.0, 1.0, 1.0); };
	virtual vec3f getPostion( const vec3f& P ) const override { return vec3f(0.0, 0.0, 0.0); };

protected:
	vec3f position;
};

class SpotLight
	: public PointLight
{
public:
	SpotLight(Scene* scene, const vec3f& pos, const vec3f& color
		, const double& const_att, const double& linear_att, const double& quad_att
		, const vec3f& dir, const double& cutoff);
	virtual double distanceAttenuation(const vec3f& P) const;
protected:
	vec3f direction;
	vec3f edgeplace;
	double cosCutoffAngle;		// should be use in both +ve and -ve
};

class WarnLight
	: public PointLight
{
public:
	WarnLight(Scene* scene, const vec3f& pos, const vec3f& color
		, const double& const_att, const double& linear_att, const double& quad_att
		, const vec3f& dir);
	virtual double distanceAttenuation(const vec3f& P) const;
protected:
	vec3f direction;
	double exponent;
};

class ShapeLight
	: public PointLight
{
public:
	ShapeLight(Scene* scene, const vec3f& pos, const vec3f& color
		, const double& const_att, const double& linear_att, const double& quad_att
		, const vec3f& dir, const vec3f& para, const vec3f& up, const vec3f& shape);
	virtual double distanceAttenuation(const vec3f& P) const;
protected:
	vec3f direction;
	vec3f parameter;
	vec3f upVector;
	vec3f shape;
	mat4f transformMatrix;
};

#endif // __LIGHT_H__
