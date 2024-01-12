////////////////////////////////////////////////////////////////////////
//
//
//  Assignment 4 of SUTD Course 50.017 (May-Aug Term, 2021)
//
//    Ray Tracing
//
//  2021-July-08
//
//
////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <iostream>

#include "SceneParser.h"
#include "Image.h"
#include "Camera.h"
#include <string.h>
#include "bitmap_image.hpp"

using namespace std;


int main( int argc, char* argv[] )
{
    // This loop loops over each of the input arguments.
    // argNum is initialized to 1 because the first "argument" provided to the program is 
    // actually the name of the executable (in our case, "Assignment_4").
    assert(argc > 1);
    for( int argNum = 1; argNum < argc; ++argNum )
    {
        std::cout << "Argument " << argNum << " is: " << argv[argNum] << std::endl;
    }
    

    //////////////////////////////////////////////////////////////////////////////////////////
    // 1. Parse the scene using SceneParser.
    //////////////////////////////////////////////////////////////////////////////////////////

    cout << "running programe" << endl;
    
    //parsing using sceneParser
    char* inputFile = nullptr;
    char* outFile = nullptr;
    int windowWidth = 0;
    int windowHeight = 0;

    for (int i = 1; i < argc; i++)
    {
         // cout << "This argument is " << argv[i] << endl;
        if (strcmp(argv[i], "-input") == 0)
        {
            cout << "This argument is " << argv[i] << endl;

            inputFile = argv[i+1];
            i ++;
        }
        else if (strcmp(argv[i], "-size") == 0)
        {
            cout << "This argument is " << argv[i] << endl;

            windowWidth = atoi(argv[i+1]);
            windowHeight = atoi(argv[i+2]);
            i++;
            i++;
        }
        else if (strcmp(argv[i], "-output") == 0)
        {
            cout << "This argument is " << argv[i] << endl;

            outFile = argv[i+1];
            i++;
        }
        else
        {
            cerr << "Usage: -input filename -size min max -output filename" << endl;
            return -1;
        }   
    }

	//// parse the input txt file
    SceneParser scene = SceneParser(inputFile);



    //////////////////////////////////////////////////////////////////////////////////////////
    // 2. Generate image by ray tracing
    //////////////////////////////////////////////////////////////////////////////////////////

    Image img = Image(windowWidth, windowHeight);

    // Loop over each pixel in the image, shooting a ray through that pixel and finding its intersection with the scene.  
    // Write the color at the intersection to that pixel in your output image.
    for (int i = 0; i<windowWidth; i++)
    {
        for (int j = 0; j<windowHeight; j++)
        {
		    //// making p from -1 to 1 in x, and -1 to 1 in y. Multiply y with inv aspect to not distort the image.
            vec2 p  = vec2( ((float(i) / (windowWidth-1)))*2 -1 , (((float(j) / (windowHeight-1)))*2 -1) * (windowHeight/windowWidth)); 
            // cout << "xcoordinate " << p[0] << " ycoordinate " << p[1] << endl;
            Ray r = scene.getCamera()->generateRay(p);
            Hit hit = Hit();
            
            if (scene.getGroup()->intersect(r, hit, scene.getCamera()->getTMin()))
            {
                vec3 colorPerPix = vec3(0,0,0);
                Material* mat = hit.getMaterial();
                
                //loop through the lights to get specular and diffuse color
                for (int k = 0; k < scene.getNumLights(); k++)
                {
                    Light* light = scene.getLight(k);
                    vec3 lightDir;
                    vec3 lightColor;
                    float distance;
                    
                    light ->getIllumination(r.pointAtParameter(hit.getT()), lightDir, lightColor, distance);

                    vec3 addColor = mat->Shade(r, hit, lightDir, lightColor);
                    colorPerPix += addColor;
                }

                colorPerPix += mat->getDiffuseColor()*scene.getAmbientLight();
                
                img.SetPixel(i, j, colorPerPix);
            }
            else
            {
                img.SetPixel(i, j, scene.getBackgroundColor());
            }
        }
    }
 
    img.SaveImage(outFile);
    
    return 0;
}

