#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>
#include "vecmath.h"

#include "Ray.h"
#include "Hit.h"
#include "texture.hpp"


class Material
{
public:
	
    Material( const vec3& d_color ,const vec3& s_color=vec3(0,0,0), float s=0):
    diffuseColor( d_color),specularColor(s_color), shininess(s)
    {

    }

    virtual ~Material()
      {

      }

    virtual vec3 getDiffuseColor() const 
    { 
      return  diffuseColor;
    }
      
    /// TODO: Implement this function to compute diffuse and specular components of Phong lighting 
    vec3 Shade( const Ray& ray, const Hit& hit, const vec3& dirToLight, const vec3& lightColor ) 
    {
        glm::vec3 n = glm::normalize(hit.getNormal());
        float nl = glm::dot(n, dirToLight);
        glm::vec3 I;

        if (nl < 0) nl = 0;

        // Diffuse lighting
        if (nl >= 0) {
          if (t.valid()) {
            glm::vec2 texCoords = glm::normalize(hit.texCoord);
            glm::vec3 textureColor = t(texCoords.x, texCoords.y);
            I += textureColor * nl * lightColor;
          }
          else {
            I += getDiffuseColor() * nl * lightColor;
          }
        }

        // Specular lighting
        glm::vec3 r = glm::normalize(2 * nl * n - dirToLight);
        float rv = glm::dot(r, -glm::normalize(ray.getDirection()));
        if (rv < 0) rv = 0;
        if (nl >= 0 && rv >= 0) {
          I += specularColor * powf(rv, shininess) * lightColor;
        }

        return I;
    }

    void loadTexture(const char * filename)
    {
      t.load(filename);
    }

    Texture t;

 protected:
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};



#endif // MATERIAL_H
