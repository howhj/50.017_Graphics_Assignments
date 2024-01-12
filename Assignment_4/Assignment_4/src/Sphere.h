#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>

#include <iostream>
using namespace std;

class Sphere: public Object3D
{
public:
	Sphere()
    { 
		//unit ball at the center
        this->center = vec3(0,0,0);
        this->radius = 1.0;
	}

	Sphere( vec3 center , float radius , Material* material ):Object3D(material)
    {
        this->center = center;
        this->radius = radius;
	}
	

	~Sphere(){}

    /// TODO: implement this function for ray-sphere intersection test.
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {
        // Construct equation at^2 + bt + c = 0
        glm::vec3 oc = r.getOrigin() - center;
        glm::vec3 d = r.getDirection();

        // Square both sides of the original ray intersection equation
        // Expand ||o + td - c||^2 and rearrange
        float a = glm::dot(d, d);
        float b = 2 * glm::dot(d, oc);
        float c = glm::dot(oc, oc) - powf(radius, 2);

        // Solve for t
        float det = powf(b, 2) - 4 * a * c;
        if (det < 0) {
            return false;
        }
        det = sqrtf(det);
        float t1 = (-b + det) / (2 * a);
        float t2 = (-b - det) / (2 * a);

        // Check for intersection
        glm::vec3 normal;
        bool rayIntersect = false;
        if (t1 >= tmin && t1 < h.getT()) {
            normal = glm::normalize(r.pointAtParameter(t1) - center);
            h.set(t1, this->material, normal);
            rayIntersect = true;
        }
        if (t2 >= tmin && t2 < h.getT()) {
            normal = glm::normalize(r.pointAtParameter(t2) - center);
            h.set(t2, this->material, normal);
            rayIntersect = true;
        }

        return rayIntersect;
	}

protected:
    vec3 center;
    float radius;
  

};


#endif
