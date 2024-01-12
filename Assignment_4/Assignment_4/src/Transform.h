#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "vecmath.h"
#include "Object3D.h"

class Transform: public Object3D
{
public: 
    Transform(){}

    Transform( const mat4& m, Object3D* obj ):o(obj)
    {
      this->m = m;
      // this->o = obj;
    }

    ~Transform()
    {

    }
    /// TODO: implement this function so that the intersect function first transforms the ray into the object's local coordindate frame
    virtual bool intersect( const Ray& r , Hit& h , float tmin)
    {
        glm::mat4 minv = glm::inverse(m);
        glm::vec4 ori(r.getOrigin(), 1.0f);
        glm::vec4 d(r.getDirection(), 0.0f);

        ori = minv * ori;
        d = minv * d;

        glm::vec3 new_o(ori.x, ori.y, ori.z);
        glm::vec3 new_d(d.x, d.y, d.z);
        new_d = glm::normalize(new_d);
        Ray new_r(new_o, new_d);

        // Transform results back to world space
        if (o->intersect(new_r, h, tmin)) {
          glm::mat4 minvT = glm::transpose(minv);
          glm::vec4 n(h.getNormal(), 0.0f);
          n = minvT * n;

          glm::vec3 new_n(n.x, n.y, n.z);
          new_n = glm::normalize(new_n);
          h.set(h.getT(), h.getMaterial(), new_n);
          return true;
        }
        return false;
    }

 protected:
    Object3D* o; //un-transformed object
    mat4 m;
};

#endif //TRANSFORM_H
