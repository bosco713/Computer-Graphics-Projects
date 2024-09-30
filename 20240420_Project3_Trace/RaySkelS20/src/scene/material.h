//
// material.h
//
// The Material class: a description of the phsyical properties of a surface
// that are used to determine how that surface interacts with light.

#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "../vecmath/vecmath.h"

#include <vector>

class Scene;
class ray;
class isect;

class Material
{
public:
    class Impulse {
    public:
        vec3f direction;
        vec3f magnitude;
        Impulse(const vec3f& d, const vec3f& m) : direction(d), magnitude(m) {}
        Impulse() {}
    };

    Material()
        : ke( vec3f( 0.0, 0.0, 0.0 ) )
        , ka( vec3f( 0.0, 0.0, 0.0 ) )
        , ks( vec3f( 0.0, 0.0, 0.0 ) )
        , kd( vec3f( 0.0, 0.0, 0.0 ) )
        , kr( vec3f( 0.0, 0.0, 0.0 ) )
        , kt( vec3f( 0.0, 0.0, 0.0 ) )
        , shininess( 0.0 ) 
		, index(1.0) {}

    Material( const vec3f& e, const vec3f& a, const vec3f& s, 
              const vec3f& d, const vec3f& r, const vec3f& t, double sh, double in)
        : ke( e ), ka( a ), ks( s ), kd( d ), kr( r ), kt( t ), shininess( sh ), index( in ) {}

	virtual vec3f shade( Scene *scene, const ray& r, const isect& i ) const;
    virtual void getBSDFImpulses(vec3f& w_i, const isect& i, vector<Impulse*>& impulseArray) const;
    virtual vec3f evaluateBSDF(vec3f& w_i, vec3f& w_o, const isect& i) const;


    virtual double computeReflectance(double cos_i, double shininess) const;
    virtual double computeTransmittance(double cos_i, double index) const;
    virtual vec3f reflectAbout(const vec3f& w_i, const vec3f& n) const;
    virtual bool refractionDirection(const vec3f& w_i, const vec3f& n, double n_i, double n_t, vec3f& refracted) const;
    virtual vec3f computeF(const vec3f& F0, double cos_i, double smoothness) const;
    virtual double glossyScatter(const vec3f& w_i, const vec3f& w_o, const vec3f& n, double smoothness) const;




    vec3f ke;                    // emissive
    vec3f ka;                    // ambient
    vec3f ks;                    // specular
    vec3f kd;                    // diffuse
    vec3f kr;                    // reflective
    vec3f kt;                    // transmissive
    
    double shininess;
    double index;               // index of refraction

    
                                // material with zero coeffs for everything
                                // as opposed to the "default" material which is
                                // a pleasant blue.
    static const Material zero;

    Material &
    operator+=( const Material &m )
    {
        ke += m.ke;
        ka += m.ka;
        ks += m.ks;
        kd += m.kd;
        kr += m.kr;
        kt += m.kt;
        index += m.index;
        shininess += m.shininess;
        return *this;
    }

    friend Material operator*( double d, Material m );
};

inline Material
operator*( double d, Material m )
{
    m.ke *= d;
    m.ka *= d;
    m.ks *= d;
    m.kd *= d;
    m.kr *= d;
    m.kt *= d;
    m.index *= d;
    m.shininess *= d;
    return m;
}
// extern Material THE_DEFAULT_MATERIAL;

#endif // __MATERIAL_H__
