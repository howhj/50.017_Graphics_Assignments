#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Object3D.h"
#include "vecmath.h"
#include <cmath>
#include <iostream>

using namespace std;

class Triangle: public Object3D
{
public:
	Triangle();
    ///@param a b c are three vertex positions of the triangle
	Triangle( const vec3& a, const vec3& b, const vec3& c, Material* m):Object3D(m)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        hasTex = false;
	}

    /// TODO: implement this function for ray-triangle intersection test.
	virtual bool intersect( const Ray& ray,  Hit& hit , float tmin)
    {
        // Construct equation Ax = b for Cramer's rule
        glm::vec3 o = ray.getOrigin();
        glm::vec3 d = ray.getDirection();

        // beta(b - a) + gamma(c - a) - td = o - a
        glm::mat3 A;
        A[0][0] = b.x - a.x;
        A[0][1] = c.x - a.x;
        A[0][2] = -d.x;
        A[1][0] = b.y - a.y;
        A[1][1] = c.y - a.y;
        A[1][2] = -d.y;
        A[2][0] = b.z - a.z;
        A[2][1] = c.z - a.z;
        A[2][2] = -d.z;

        glm::vec3 b(o.x - a.x, o.y - a.y, o.z - a.z);
        glm::vec3 x;

        // Cramer's rule
        for (int i = 0; i < 3; i++) {
            // Construct Ai by copying A
            glm::mat3 Ai;
            for (int j = 0; j < 3; j++) {
                for (int k = 0; k < 3; k++) {
                    Ai[j][k] = A[j][k];
                }
            }

            // Replace i-th column with b
            for (int j = 0; j < 3; j++) {
                Ai[j][i] = b[j];
            }

            // Solve for variables
            x[i] = glm::determinant(Ai) / glm::determinant(A);
        }

        float alpha = 1 - x.x - x.y;
        float beta = x.x;
        float gamma = x.y;
        if (alpha < 0 || beta < 0 || gamma < 0) {
            return false;
        }
        float t = x.z;

        // Check for intersection
        if (t >= tmin && t < hit.getT()) {
            glm::vec3 normal = glm::normalize(alpha * normals[0] + beta * normals[1] + gamma * normals[2]);
            hit.set(t, this->material, normal);

            // Set texture coordinates
            glm::vec2 texture = glm::normalize(alpha * texCoords[0] + beta * texCoords[1] + gamma * texCoords[2]);
            hit.setTexCoord(texture);
            return true;
        }

        return false;
    }
    
	bool hasTex;
	vec3 normals[3];
	vec2 texCoords[3];
    
protected:
    vec3 a;
    vec3 b;
    vec3 c;

};

#endif //TRIANGLE_H
