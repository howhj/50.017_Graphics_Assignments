#ifndef SCENE_PARSER_H
#define SCENE_PARSER_H

#include <cassert>
#include "vecmath.h"

#include "SceneParser.h"
#include "Camera.h"
#include "Light.h"
#include "Material.h"
#include "Object3D.h"
#include "Mesh.hpp"
#include "Group.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Transform.h"

/*
class Camera;
class Light;
class Material;
class Object3D;
class Group;
class Sphere;
class Plane;
class Triangle;
class Transform;
*/
#define MAX_PARSER_TOKEN_LENGTH 100

class SceneParser
{
public:

    SceneParser( const char* filename );
    ~SceneParser();

    Camera* getCamera() const
    {
        return camera;
    }

    vec3 getBackgroundColor() const
    {
        return background_color;
    }

    vec3 getAmbientLight() const
    {
        return ambient_light;
    }
    
    int getNumLights() const
    {
        return num_lights;
    }
    
    Light* getLight( int i ) const
    {
        assert( i >= 0 && i < num_lights );
        return lights[i];
    }

    int getNumMaterials() const
    {
        return num_materials;
    }

    Material* getMaterial( int i ) const
    {
        assert( i >= 0 && i < num_materials );
        return materials[i];
    }

    Group* getGroup() const
    {
        return group;
    }

private:

    SceneParser()
    {
        assert( false );
    }

    void parseFile();
    void parsePerspectiveCamera();
    void parseBackground();
    void parseLights();
    Light* parseDirectionalLight();
	Light* parsePointLight();
    void parseMaterials();
    Material* parseMaterial();

    Object3D* parseObject( char token[ MAX_PARSER_TOKEN_LENGTH ] );
    Group* parseGroup();
    Sphere* parseSphere();
    Plane* parsePlane();
    Triangle* parseTriangle();
    Mesh* parseTriangleMesh();
    Transform* parseTransform();

    int getToken( char token[ MAX_PARSER_TOKEN_LENGTH ] );
    vec3 readvec3();
    vec2 readVec2f();
    float readFloat();
    int readInt();

    FILE* file;
    Camera* camera;
    vec3 background_color;
    vec3 ambient_light;
    int num_lights;
    Light** lights;
    int num_materials;
    Material** materials;
    Material* current_material;
    Group* group;
};

#endif // SCENE_PARSER_H
