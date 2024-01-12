#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Joint
{
	glm::mat4 transform; // transform relative to its parent
	std::vector< Joint* > children; // list of children
};

#endif
