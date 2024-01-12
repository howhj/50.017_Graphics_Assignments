#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>
using namespace std;


class Plane: public Object3D
{
public:
	Plane(){}

	Plane( const vec3& normal , float d , Material* m):Object3D(m)
    {
        this->normal = normalize(normal);
        this->d = -d;
	}

	~Plane(){}

    /// TODO: implement this function for ray-plane intersection test.
	virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {   
        // Construct equation at + b = 0
        float no = glm::dot(normal, r.getOrigin());
        float nd = glm::dot(normal, r.getDirection());

        // Solve for t
        if (nd == 0) {
            return false;
        }
        float t = -(no + d) / nd;

        // Check for intersection
        if (t >= tmin && t < h.getT()) {
            h.set(t, this->material, normal);
            return true;
        }
        return false;
	}

protected:
    vec3 normal;
    float d;

    
};
#endif //PLANE_H
		

