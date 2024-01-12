#ifndef SKELETALMODEL_H
#define SKELETALMODEL_H
#define GLM_ENABLE_EXPERIMENTAL

#ifdef WIN32
#include <windows.h>
#endif

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

#include <cstdlib>


#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include "Joint.h"
#include "MatrixStack.h"

using namespace std;


class SkeletalModel
{
public:

	// Part 1: Implement method to load a skeleton.
	// This method should compute m_rootJoint and populate m_joints.
	void loadSkeleton( const char* filename );

	// Part 2: Compute joint/bone transformations for drawing the skeleton.
	void computeTransforms();

	// 2.1. Implement this method with a recursive helper to draw a sphere at each joint.
	void computeJointTransforms( );
	void computeJointTransforms(Joint* joint, MatrixStack matrixStack);

	// 2.2. Implement this method with a recursive helper to draw a cylinder between each pair of joints
	void computeBoneTransforms( );
	void computeBoneTransforms(Joint* joint,  MatrixStack matrixStack);

	// Part 3: Implement this method to handle changes to your skeleton given changes of joints angles
	void setJointTransform( int jointIndex, float rX, float rY, float rZ );

private:
	// pointer to the root joint
	Joint* m_rootJoint;

	// the list of joints
	std::vector< Joint* > m_joints;

	// matrix stack for joint/bone transformations
    MatrixStack m_matrixStack;


public:
	// a vector that stores the local transformation matrix of each joint (i.e., ball)
    vector<glm::mat4> jointMatList;

	// a vector that stores the local transformation matrix of each bone (i.e., cylinder)
	vector<glm::mat4> boneMatList;
};

#endif
