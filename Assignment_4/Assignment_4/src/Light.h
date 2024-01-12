#ifndef LIGHT_H
#define LIGHT_H

#include <cstdlib>
#include <cstdio>
#include <cstring>

//#include "vec3.h"
#include "vecmath.h"
#include "Object3D.h"
#include "Image.h"
#include <math.h>

class Light
{
public:

    Light()
    {

    }

    virtual ~Light()
    {

    }

    virtual void getIllumination( const vec3& p, vec3& dir, vec3& col, float& distanceToLight ) const = 0;

};

class DirectionalLight : public Light
{
public:

    DirectionalLight( const vec3& d, const vec3& c )
    {
        direction = normalize(d);
        color = c;
    }

    ~DirectionalLight()
    {

    }
    ///@param p unsed in this function
    ///@param distanceToLight not well defined because it's not a point light
    virtual void getIllumination( const vec3& p, vec3& dir, vec3& col, float& distanceToLight ) const
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
        dir = -direction;
        col = color;
    }

private:

    DirectionalLight(); // don't use

    vec3 direction;
    vec3 color;

};

class PointLight : public Light
{
public:

    PointLight( const vec3& p, const vec3& c )
    {
        position = p;
        color = c;
    }

    ~PointLight()
    {

    }

    virtual void getIllumination( const vec3& p, vec3& dir, vec3& col, float& distanceToLight ) const
    {
        // the direction to the light is the opposite of the
        // direction of the directional light source
		dir = (position-p);
		float length = sqrt(dir[0] * dir[0] + dir[1] * dir[1] + dir[2] * dir[2]);
		dir = dir / length;
        col = color;
    }

private:

    PointLight(); // don't use

    vec3 position;
    vec3 color;

};

#endif // LIGHT_H
