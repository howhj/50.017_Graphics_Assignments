///=========================================================================================///
///
///                       Functions to be filled in for Assignment 2   
///
///           IMPORTANT: you ONLY need to work on functions with TODO in this section    
///    
///=========================================================================================///


#include "SkeletalModel.h"


///=========================================================================================///
///                                    Load .skel File    
///=========================================================================================///


// DONE: Load the skeleton from file here, create hierarchy of joints
//       (i.e., set values for m_rootJoint and m_joints)
void SkeletalModel::loadSkeleton( const char* filename )
{
   FILE* pFile = fopen(filename, "r");
   if(pFile == NULL) {
       perror ("Error opening file.");
    }
    else {
        while (!feof (pFile)) {
            // Parse each line
            glm::vec3 translation;
            int parent;
            fscanf(pFile, "%f %f %f %d\n", &translation.x, &translation.y, &translation.z, &parent);

            // Process transformation and parent for each joint
            // Credit to Keng Hoy for pointing out the need to transpose the matrix
            Joint* joint = new Joint;
            joint->transform = glm::transpose(glm::translate(glm::mat4(1.0f), translation));
            m_joints.push_back(joint);
            if (parent == -1) {
                m_rootJoint = joint;
            }
            else {
                m_joints[parent]->children.push_back(joint);
            }
        }
    }

    fclose(pFile);
}




///=========================================================================================///
///                         Compute transformations for Joints and Bones     
///=========================================================================================///

void SkeletalModel::computeTransforms()
{
    if( m_joints.size() == 0 )
        return;

    computeJointTransforms();

    computeBoneTransforms();
}

// Compute a transformation matrix for each joint (i.e., ball) of the skeleton
void SkeletalModel::computeJointTransforms( )
{
    jointMatList.clear();

    m_matrixStack.clear();

    computeJointTransforms(m_rootJoint, m_matrixStack);
}

// DONE: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the joints
void SkeletalModel::computeJointTransforms(Joint* joint, MatrixStack matrixStack)
{
   matrixStack.push(joint->transform);

   for (int i = 0; i < joint->children.size(); i++) {
       computeJointTransforms(joint->children[i], matrixStack);
    }

    jointMatList.push_back(matrixStack.top());
    matrixStack.pop();
}


// Compute a transformation matrix for each bone (i.e., cylinder) between each pair of joints in the skeleton
void SkeletalModel::computeBoneTransforms( )
{
    boneMatList.clear();

    m_matrixStack.clear();

    computeBoneTransforms(m_rootJoint, m_matrixStack);
}

// DONE: You will need to implement this recursive helper function to traverse the joint hierarchy for computing transformations of the bones
void SkeletalModel::computeBoneTransforms(Joint* joint, MatrixStack matrixStack)
{
    matrixStack.push(joint->transform);

    for (int i = 0; i < joint->children.size(); i++) {
        // Translation
        glm::mat4 boneMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.5f));
        boneMatrix = glm::transpose(boneMatrix);

        // Scaling
        // Credit to Keng Hoy for reminding me that the parentOffset is the same as the
        // child's translation vector, since it is already relative to the parent
        Joint* child = joint->children[i];
        glm::mat4 temp = child->transform;
        glm::vec3 parentOffset(temp[0][3], temp[1][3], temp[2][3]);
        float distance = glm::length(parentOffset);
        boneMatrix = glm::scale(boneMatrix, glm::vec3(0.01f, 0.01f, distance));

        // Rotation
        glm::vec3 target = glm::normalize(parentOffset);
        glm::vec3 z(0.0f, 0.0f, 1.0f);
        float angle = acos(glm::dot(target, z));
        glm::vec3 axis =  glm::cross(target, z);
        boneMatrix = glm::rotate(boneMatrix, angle, axis);

        // Compute the bone's transformation in world space'
        matrixStack.push(boneMatrix);
        boneMatList.push_back(matrixStack.top());
        matrixStack.pop();

        // Recursively process the child joint
        computeBoneTransforms(child, matrixStack);
    }

    matrixStack.pop();
}




///=========================================================================================///
///                              Set Joint Angles for Transform     
///=========================================================================================///

// DONE: Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
void SkeletalModel::setJointTransform(int jointIndex, float angleX, float angleY, float angleZ)
{
    // Rotation matrix for each axis
    glm::mat4 xMat(1.0f);
    xMat[1][1] = cos(angleX);
    xMat[1][2] = -sin(angleX);
    xMat[2][1] = sin(angleX);
    xMat[2][2] = cos(angleX);

    glm::mat4 yMat(1.0f);
    yMat[0][0] = cos(angleY);
    yMat[0][2] = sin(angleY);
    yMat[2][0] = -sin(angleY);
    yMat[2][2] = cos(angleY);

    glm::mat4 zMat(1.0f);
    zMat[0][0] = cos(angleZ);
    zMat[0][1] = -sin(angleZ);
    zMat[1][0] = sin(angleZ);
    zMat[1][1] = cos(angleZ);

    // Combine the rotation matrices and the joint's translation vector
    glm::mat4 rotMat = xMat * yMat * zMat;
    glm::mat4 transform = m_joints[jointIndex]->transform;
    for (int i = 0; i < 3; i++) rotMat[i][3] = transform[i][3];
    m_joints[jointIndex]->transform = rotMat;
}







