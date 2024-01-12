////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 1 of SUTD Course 50.017 (May-Aug Term, 2022)
//
//    Mesh Viewer
//
//  2022-May-26
//
//
////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include "shaderSource.h"
#include "shader.h"


using namespace std;


#define MAX_BUFFER_SIZE            1024

#define _ROTATE_FACTOR              0.005f
#define _SCALE_FACTOR               0.01f
#define _TRANS_FACTOR               0.02f

#define _Z_NEAR                     0.001f
#define _Z_FAR                      100.0f



/***********************************************************************/
/**************************   global variables   ***********************/
/***********************************************************************/


// Window size
unsigned int winWidth  = 800;
unsigned int winHeight = 600;

// Camera
glm::vec3 camera_position = glm::vec3 (0.0f, 0.0f, 15.0f);
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

 // Mesh color table
glm::vec3 colorTable[4] = 
 {
    glm::vec3(0.6, 1.0, 0.6),
    glm::vec3(1.0, 0.6, 0.6),
    glm::vec3(0.6, 0.6, 1.0),
    glm::vec3(1.0, 1.0, 0.6) 
};

// Mesh rendering color
int colorID = 0;
glm::vec3  meshColor;


// declaration
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);





/******************************************************************************/
/***************   Functions to be filled in for Assignment 1    **************/
/***************    IMPORTANT: you ONLY need to work on these    **************/
/***************                functions in this section        **************/
/******************************************************************************/


// DONE: insert your code in this function for Mesh Loading
//       1) store vertices and normals in verList with order (v.x, v.y, v.z, n.x, n.y, n.z)
//       2) store vertex indices of each triangle in triList 
int LoadInput(vector<float> &verList, vector<unsigned> &triList)
{
    bool file_is_open = false;
    FILE* file;
    while (!file_is_open) {
        // Ask for user input for file name of OBJ file
        char filepath[1024];
        cout << "Enter the path to an OBJ file: ";
        cin.getline(filepath, 1024);
        cout << filepath << endl;

        FILE* f = fopen(filepath, "r");
        if (f != NULL) {
            file_is_open = true;
            file = f;
        }
        else {
            cout << "Error opening file. Did you provide the correct file path?" << endl;
            cout << endl;
        }
    }

    // Adapted from: https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/
    vector<glm::vec3> temp_n;


    while (true) {
        char header[128];
        int res = fscanf(file, "%s", header);
        if (res == EOF)
            break;

        if (strcmp (header, "v") == 0) {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            verList.push_back(vertex.x);
            verList.push_back(vertex.y);
            verList.push_back(vertex.z);
            for (int i = 0; i < 3; i++)
                verList.push_back(0.0f);
        }
        else if (strcmp (header, "vn") == 0) {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_n.push_back(normal);
        }
        else if (strcmp (header, "f") == 0) {
            string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], textureIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textureIndex[0], &normalIndex[0], &vertexIndex[1], &textureIndex[1], &normalIndex[1], &vertexIndex[2], &textureIndex[2], &normalIndex[2]);
            if (matches != 9) {
                cout << "Error reading file. Please terminate with ctrl+c" << endl;
                return 1;
            }

            for (int i = 0; i < 3; i++) {
                triList.push_back(vertexIndex[i] - 1);

                int idx = (vertexIndex[i] - 1) * 6 + 3;
                verList[idx] = temp_n[normalIndex[i] - 1].x;
                verList[idx + 1] = temp_n[normalIndex[i] - 1].y;
                verList[idx + 2] = temp_n[normalIndex[i] - 1].z;
            }
        }
    }

    // Original attempt that did not load the mesh properly
    /*
    // Open OBJ file and read from it
    ifstream file (filename);

    if (!file.is_open()) {
        cout << "Error opening file. Please terminate with ctrl+c" << endl;
        return 1;
    }
    // Create array for normals only
    vector<float> normals;

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        float x, y, z;
        string header;
        if (iss >> header >> x >> y >> z) {
            // While lines start with v, add to verList
            if (header == "v") {
                verList.push_back(x);
                verList.push_back(y);
                verList.push_back(z);
                for (int i; i < 3; i++) {
                    verList.push_back(0.0f);
                }
            }

            // While lines start with vn, add to normals vertex
            else if (header == "vn") {
                normals.push_back(x);
                normals.push_back(y);
                normals.push_back(z);
            }
        }

        // Add data from remaining lines to triList
        // Use face data to insert normals into verList correctly
        else {
            // Create a new istringstream since iss has already parsed through the line during the if check
            istringstream ls(line);
            string f1, f2, f3;

            // Split the data triplets into 3 strings
            if (ls >> header >> f1 >> f2 >> f3 && header == "f") {
                unsigned int a, c;
                string b;
                int idx;
                int ctr = 0;

                istringstream iss1(f1);
                istringstream iss2(f2);
                istringstream iss3(f3);

                // Parse through the data triplet with / as the delimiter
                while (getline(iss1, b, '/')) {
                    if (ctr == 0) {
                        // Create a new istringstream to convert string to unsigned int
                        // The methods I found online required the string library, which
                        // I'm not sure if I'm allowed to import
                        istringstream bs(b);
                        bs >> a;
                    }
                    else if (ctr == 2) {
                        istringstream bs(b);
                        bs >> c;

                        // Add vertex index to triList
                        triList.push_back(a - 1);

                        // Insert normal data into verList
                        idx = (a - 1) * 6 + 3;
                        for (int k = 0; k < 3; k++) {
                            if (verList[idx + k] == 0.0f) {
                                verList[idx + k] = normals[(c - 1) * 3 + k];
                            }
                        }
                        // Reset ctr
                        ctr = -1;
                    }
                    ctr++;
                }

                // The rest is just boilerplate code for the other 2 triplets
                // This is to keep everything within a single function
                // since I'm not sure if I am allowed to write helper functions
                while (getline(iss2, b, '/')) {
                    if (ctr == 0) {
                        istringstream bs(b);
                        bs >> a;
                    }
                    else if (ctr == 2) {
                        istringstream bs(b);
                        bs >> c;
                        triList.push_back(a - 1);
                        idx = (a - 1) * 6 + 3;
                        for (int k = 0; k < 3; k++) {
                            if (verList[idx] == 0.0f) {
                                verList[idx + k] = normals[(c - 1) * 3 + k];
                            }
                        }
                        ctr = -1;
                    }
                    ctr++;
                }

                while (getline(iss3, b, '/')) {
                    if (ctr == 0) {
                        istringstream bs(b);
                        bs >> a;
                    }
                    else if (ctr == 2) {
                        istringstream bs(b);
                        bs >> c;
                        triList.push_back(a - 1);
                        idx = (a - 1) * 6 + 3;
                        for (int k = 0; k < 3; k++) {
                            if (verList[idx] == 0.0f) {
                                verList[idx + k] = normals[(c - 1) * 3 + k];
                            }
                        }
                        ctr = -1;
                    }
                    ctr++;
                }
            }
        }
    }
    */

    // Note: these two lines of code is to avoid runtime error; 
    //       please remove them after you fill your own code for 3D model loading
    //verList.push_back(0);
    //triList.push_back(0);
    
    return 0;
}

// DONE: insert your code in this function for Mesh Coloring
void SetMeshColor(int &colorID)
{
   meshColor = colorTable[colorID];
   colorID++;
   if (colorID >= 4)
       colorID = 0;
}

// Reference: https://learnopengl.com/Getting-started/Transformations
// DONE: insert your code in this function for Mesh Transformation (Rotation)
void RotateModel(float angle, glm::vec3 axis)
{
    modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
}

// DONE: insert your code in this function for Mesh Transformation (Translation)
void TranslateModel(glm::vec3 transVec)
{
    modelMatrix = glm::translate(modelMatrix, transVec);
}

// DONE: insert your code in this function for Mesh Transformation (Scaling)
void ScaleModel(float scale)
{
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale, scale, scale));
}




/******************************************************************************/
/***************                  Callback Function              **************/
/******************************************************************************/

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


// glfw: whenever a key is pressed, this callback is called
// ----------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        SetMeshColor( colorID );
    }
}


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


// glfw: whenever the cursor moves, this callback is called
// -------------------------------------------------------
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


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
	float scale = 1.0f + _SCALE_FACTOR * yOffset;

	ScaleModel( scale ); 
}




/******************************************************************************/
/***************                    Main Function                **************/
/******************************************************************************/

int main()
{
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
    GLFWwindow* window = glfwCreateWindow(winWidth, winHeight, "Assignment 1 - Mesh Viewer", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
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

    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    shader myShader;
    myShader.setUpShader(vertexShaderSource,fragmentShaderSource);

    // Load input mesh data
    vector<float> verList;          // This is the list of vertices and normals for rendering
    vector<unsigned> triList;       // This is the list of faces for rendering
    LoadInput(verList, triList);

    // create buffers/arrays
    unsigned int VBO, VAO,EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, verList.size() * sizeof(float), &verList[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, triList.size() * sizeof(unsigned int), &triList[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normals
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), ((void*)(3* sizeof(float))));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    // as we only have a single shader, we could also just activate our shader once beforehand if we want to 
    myShader.use();


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // view/projection transformations
        projection = glm::perspective(glm::radians(camera_fovy), (float)winWidth / (float)winHeight, _Z_NEAR, _Z_FAR);
        glm::mat4 view = glm::lookAt(camera_position, camera_target, camera_up);

        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "projection"), 1, GL_FALSE, &projection[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "view"), 1, GL_FALSE, &view[0][0]);

        // render the loaded model
        //glm::vec3 aColor = glm::vec3 (0.6f, 1.0f, 0.6f);
        glUniformMatrix4fv(glGetUniformLocation(myShader.ID, "model"), 1, GL_FALSE, &modelMatrix[0][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "meshColor"), 1, &colorTable[colorID][0]);
        glUniform3fv(glGetUniformLocation(myShader.ID, "viewPos"), 1, &camera_position[0]);

        // render the triangle
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, triList.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(myShader.ID);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

