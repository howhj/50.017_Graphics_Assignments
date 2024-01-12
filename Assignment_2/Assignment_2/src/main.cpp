////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 2 of SUTD Course 50.017 (May-Aug Term, 2021)
//
//    Hierarchical Skeleton
//
//  2021-June-10
//
//
////////////////////////////////////////////////////////////////////////

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include "math.h"
#include "shaderSource.h"
#include "shader.h"
#include "SkeletalModel.h"
#include "Sphere.h"
#include "Cylinder.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;



#define _ROTATE_FACTOR              0.005f
#define _SCALE_FACTOR               0.005f
#define _TRANS_FACTOR               0.003f

#define _Z_NEAR                     0.001f
#define _Z_FAR                      100.0f



/***********************************************************************/
/**************************   global variables   ***********************/
/***********************************************************************/

// declaration
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);

// Window size
unsigned int winWidth  = 800;
unsigned int winHeight = 600;

// Camera
glm::vec3 camera_position = glm::vec3 (0.0f, 0.0f, 2.5f);
glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 camera_up = glm::vec3(0.0f, 1.0f, 0.0f);
float camera_fovy = 45.0f;    
glm::mat4 projection;

// Mouse interaction
bool leftMouseButtonHold = false;
bool isFirstMouse = true;
float prevMouseX;
float prevMouseY;
glm::mat4 modelMatrix = glm::mat4(1.0f);

// Skeletal model
SkeletalModel myModel;



///=========================================================================================///
///                                    Load SKEL file
///=========================================================================================///

// Load the .skel file here
void LoadInput()
{
    //local variables
    string inputString("../data/Model4.skel");

    cout << "Please enter filename.skel: ";
    cin >> inputString;
    cout << "Displaying: " << inputString <<endl;

    myModel.loadSkeleton(inputString.data());
}




///=========================================================================================///
///                             Functions for Rendering 3D Model 
///=========================================================================================///

void SetSphereData(Sphere &my_sphere, unsigned int &sphere_VAO, unsigned int &sphere_VBO) 
{
    my_sphere.set(0.020, 50, 50);

    glGenVertexArrays(1, &sphere_VAO);
    glGenBuffers(1, &sphere_VBO);

    glBindVertexArray(sphere_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
                 my_sphere.getInterleavedVertexSize(), // data size, # of bytes
                 my_sphere.getInterleavedVertices(),   // ptr to vertex data
                 GL_STATIC_DRAW);                   // usage

    // copy index data to VBO
    unsigned int sphere_iboId;
    glGenBuffers(1, &sphere_iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_iboId);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
                 my_sphere.getIndexSize(),             // data size, # of bytes
                 my_sphere.getIndices(),               // ptr to index data
                 GL_STATIC_DRAW);                   // usage


    // bind VBOs
    glBindBuffer(GL_ARRAY_BUFFER, sphere_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere_iboId);

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    //int stride = sphere.getInterleavedStride();     // should be 32 bytes
    glVertexAttribPointer(0,   3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1,   3, GL_FLOAT, false, 6 * sizeof(float), (void*)(sizeof(float)*3));
}

void SetCylinderData(Cylinder &my_cylinder, unsigned int &cylinder_VAO, unsigned int &cylinder_VBO) 
{
    my_cylinder.set(1.0, 1.0, 1.0, 36, 1);

    glGenVertexArrays(1, &cylinder_VAO);
    glGenBuffers(1, &cylinder_VBO);

    glBindVertexArray(cylinder_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, cylinder_VBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
                 my_cylinder.getInterleavedVertexSize(), // data size, # of bytes
                 my_cylinder.getInterleavedVertices(),   // ptr to vertex data
                 GL_STATIC_DRAW);                   // usage

    // copy index data to VBO
    unsigned int cylinder_iboId;
    glGenBuffers(1, &cylinder_iboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder_iboId);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
                 my_cylinder.getIndexSize(),             // data size, # of bytes
                 my_cylinder.getIndices(),               // ptr to index data
                 GL_STATIC_DRAW);                   // usage

    // bind VBOs
    glBindBuffer(GL_ARRAY_BUFFER, cylinder_VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cylinder_iboId);

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    //int stride = sphere.getInterleavedStride();     // should be 32 bytes
    glVertexAttribPointer(0,   3, GL_FLOAT, false, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1,   3, GL_FLOAT, false, 6 * sizeof(float), (void*)(sizeof(float)*3));

}





///=========================================================================================///
///                            Functions for Manipulating 3D Model  
///=========================================================================================///

void RotateModel(float angle, glm::vec3 axis)
{
    glm::vec3 rotateCenter = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    glm::mat4 rotateMatrix = glm::mat4(1.0f);
    rotateMatrix = glm::translate(rotateMatrix, rotateCenter);
    rotateMatrix = glm::rotate(rotateMatrix, angle, axis);
    rotateMatrix = glm::translate(rotateMatrix, -rotateCenter);

    modelMatrix = rotateMatrix * modelMatrix;
}

void TranslateModel(glm::vec3 transVec)
{
    glm::mat4 translateMatrix = glm::mat4(1.0f);
    translateMatrix = glm::translate(translateMatrix, transVec);

    modelMatrix = translateMatrix * modelMatrix;
}

void ScaleModel(float scale)
{
    glm::vec3 scaleCenter = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    glm::mat4 scaleMatrix = glm::mat4(1.0f);
    scaleMatrix = glm::translate(scaleMatrix, scaleCenter);
    scaleMatrix = glm::scale(scaleMatrix, glm::vec3(scale, scale, scale));
    scaleMatrix = glm::translate(scaleMatrix, -scaleCenter);

    modelMatrix = scaleMatrix * modelMatrix;
}




///=========================================================================================///
///                                    Callback Functions
///=========================================================================================///

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}


// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.

    glViewport(0, 0, width, height);

    winWidth  = width;
    winHeight = height;
}


// This function is called whenever a "Normal" key press is received.
// ---------------------------------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        cout << "input the joint index from the list:" << endl;
        for (size_t i = 0; i < myModel.jointMatList.size(); i++)
        {
            cout << i << " ";
        }
        cout << endl;
        int chosenIndex;
        cin >> chosenIndex;

        printf("selected joint index: %d \n", chosenIndex);

        if (chosenIndex >= 0 && chosenIndex < myModel.jointMatList.size())
        {
            float jointRotateAngle[3];
            cout << "choose rotation angle for x, y,z axis, range: [-180, 180] (example: 90, 90, 120)" << endl;
            cout << "input the rotation angle for X axis:" << endl;
            cin >> jointRotateAngle[0];
            cout << "input the rotation angle for Y axis:" << endl;
            cin >> jointRotateAngle[1];
            cout << "input the rotation angle for Z axis:" << endl;
            cin >> jointRotateAngle[2];

            if (jointRotateAngle[0] >= -180 && jointRotateAngle[0] <= 180 &&
                jointRotateAngle[1] >= -180 && jointRotateAngle[1] <= 180 &&
                jointRotateAngle[2] >= -180 && jointRotateAngle[2] <= 180)
            {
                myModel.setJointTransform(chosenIndex, jointRotateAngle[0], jointRotateAngle[1], jointRotateAngle[2]);
                myModel.computeTransforms();
            }
            else
            {
                cout << "wrong input angles" << endl;
            }
        }
        else
        {
            cout << "wrong joint index" << endl;
        }
    }
}


// glfw: whenever the mouse button is clicked, this callback is called
// ---------------------------------------------------------
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        leftMouseButtonHold = true;
    }
    else
    {
        leftMouseButtonHold = false;
    }
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yOffset)
{
    float scale = 1.0f + _SCALE_FACTOR * yOffset;

    ScaleModel( scale );
}


// glfw: whenever the cursor moves, this callback is called
// ---------------------------------------------------------
void cursor_pos_callback(GLFWwindow* window, double mouseX, double mouseY)
{
    float  dx, dy;
    float  nx, ny, scale, angle;


    if ( leftMouseButtonHold )
    {
        if (isFirstMouse)
        {
            prevMouseX = mouseX;
            prevMouseY = mouseY;
            isFirstMouse = false;
        }

        else
        {
            if( glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS )
            {
                float dx =         _TRANS_FACTOR * (mouseX - prevMouseX);
                float dy = -1.0f * _TRANS_FACTOR * (mouseY - prevMouseY); // reversed since y-coordinates go from bottom to top

                prevMouseX = mouseX;
                prevMouseY = mouseY;

                TranslateModel( glm::vec3(dx, dy, 0) );
            }

            else
            {
                float dx =   mouseX - prevMouseX;
                float dy = -(mouseY - prevMouseY); // reversed since y-coordinates go from bottom to top

                prevMouseX = mouseX;
                prevMouseY = mouseY;

                // Rotation
                nx    = -dy;
                ny    =  dx;
                scale = sqrt(nx*nx + ny*ny);

                // We use "ArcBall Rotation" to compute the rotation axis and angle based on the mouse motion
                nx    = nx / scale;
                ny    = ny / scale;
                angle = scale * _ROTATE_FACTOR;

                RotateModel( angle, glm::vec3(nx, ny, 0.0f) );
            }
        }
    }

    else
    {
        isFirstMouse = true;
    }

}




///=========================================================================================///
///                                      Main Function
///=========================================================================================///

int main()
{
    LoadInput();
    myModel.computeTransforms();

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Assignment 2 - Hierarchical Skeleton", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // tell GLFW to capture the mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // construct the shaders
    shader myShader;
    myShader.setUpShader(vertexShaderSource,fragmentShaderSource);

    unsigned int sphere_VAO, sphere_VBO;
    Sphere my_sphere;
    SetSphereData(my_sphere, sphere_VAO, sphere_VBO);

    unsigned int cylinder_VAO, cylinder_VBO;
    Cylinder my_cylinder;
    SetCylinderData(my_cylinder, cylinder_VAO, cylinder_VBO);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // ------
        glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //// render mesh
        myShader.use();

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera_fovy), (float)winWidth / (float)winHeight, _Z_NEAR, _Z_FAR);
        glm::mat4 view = glm::lookAt(camera_position, camera_target, camera_up);

        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        glm::vec3 aColor = glm::vec3 (0.6f, 1.0f, 0.6f);

        glUniform3fv(glGetUniformLocation(myShader.ID, "aColor"), 1, &aColor[0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "viewPos"), 1, &camera_position[0]);

        // draw the skeleton
        if( myModel.jointMatList.size() > 0 )
        {
            int rootJointIndex = myModel.jointMatList.size() - 1;
            glm::mat4 modelCenterMat = glm::transpose( myModel.jointMatList[rootJointIndex] );

            // draw all the joints
            for (int i = 0; i < myModel.jointMatList.size(); ++i)
            {
                glm::mat4 jointLocaMat = glm::transpose(myModel.jointMatList[i]);
                glm::mat4 jointGlobMat = modelCenterMat * modelMatrix *  glm::inverse(modelCenterMat) * jointLocaMat;

                glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &jointGlobMat[0][0]);;

                glBindVertexArray(sphere_VAO);
                glDrawElements(GL_TRIANGLES, my_sphere.getIndexCount(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }

            // draw all the bones
            for (int i = 0; i < myModel.boneMatList.size(); ++i) 
            {
                glm::mat4 boneLocaMat = glm::transpose(myModel.boneMatList[i]);
                glm::mat4 boneGlobMat = modelCenterMat * modelMatrix *  glm::inverse(modelCenterMat) * boneLocaMat;

                glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &boneGlobMat[0][0]);;

                glBindVertexArray(cylinder_VAO);
                glDrawElements(GL_TRIANGLES, my_cylinder.getIndexCount(), GL_UNSIGNED_INT, 0);
                glBindVertexArray(0);
            }   
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &sphere_VAO);
    glDeleteVertexArrays(1, &cylinder_VAO);
    glDeleteBuffers(1, &sphere_VBO);
    glDeleteBuffers(1, &cylinder_VBO);
    glDeleteProgram(myShader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}




