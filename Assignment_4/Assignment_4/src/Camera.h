#ifndef CAMERA_H
#define CAMERA_H

#include "Ray.h"
#include "vecmath.h"
#include <float.h>
#include <cmath>



class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const vec2& point ) = 0 ; 
	
	virtual float getTMin() const = 0 ; 
	virtual ~Camera(){}
protected:
	vec3 center; 
	vec3 direction;
	vec3 up;
	vec3 horizontal;

};


class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const vec3& center, const vec3& direction,const vec3& up , float angle)
    {
        this->center = center;
        this->direction = direction;
        this->up = up;
        this->angle = angle;
        this->w = normalize(direction);
        this->u = normalize(cross(w, up));
        this->v = normalize(cross(u, w));    
	}

	virtual Ray generateRay( const vec2& point)
    {
        // get ray direction 
        float d = 1.0f/ tanf(angle/2.0f);
        vec3 direction = normalize((u*point[0] + v*point[1] + d*w));
        
        return Ray(center, direction);
    }

	virtual float getTMin() const 
    { 
		return 0.0f;
	}

private:
    float angle;
    vec3 w;
    vec3 u;
    vec3 v;
};

#endif //CAMERA_H
