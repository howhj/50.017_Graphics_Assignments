# 50.017 Computer Graphics Assignments

This repo contains the 4 programming assignments of the 50.017 course in term 6.

## Instructions

The code for the assignments are written in C++, using OpenGL to render 3D mesh models from OBJ files. To run the code for assignment `{n}`:

1. `cd {repo}/Assignment_{n}/Assignment_{n}/build`
2. `cmake ..`
3. `make`
4. `./Assignment_{n}`
5. Follow the instructions in the CLI to enter the input file

Input files can be located in `{repo}/Assignment_{n}/Assignment_{n}/data`.

### Special Instructions for Assignment 4

For assignment 4, step 4 is different. Do the following instead:

`./Assignment_4 -input {input file} -size {width} {height} -output {output file}`

Note that the input expects txt files, not obj files.

To avoid unexpected behaviour, use the same value for both the width and the height. It is also recommended that the width and height values are small (e.g. 200) to minimise the time taken to produce the image.

Wait for the process to finish, then check the output file.

#### Alternative Method

To automatically produce the images for all available inputs, run the following after building:

`{repo}/Assignment_4/Assignment_4/script.sh`

The output images are stored in:

`{repo}/Assignment_4/Assignment_4/image`

## Assignment 1: Loading Models and Transformations

Assignment 1 is about parsing OBJ files, extracting the vertex, normal and face data, and rendering 3D mesh models.

The model can be interacted with using the mouse, by applying transformations to the model.

* Rotation: left click and drag
* Translation: shift + left click and drag
* Scaling: scroll

It is also possible to cycle through a number of colours for the model, by pressing C.

## Assignment 2: Hierarchical Models

This assignment is about working with objects in a hierarchy. To represent this, a skeleton with a number of joints and bones arranged in a hierarchy is used. To draw each object, the parent object's transformation is used as a basis.

The skeleton's pose can be manipulated, by applying rotations to different joints. To do so, press the C key to bring up a prompt on the terminal, then follow the instructions to enter a joint ID and the angle of rotation for each axis.

Like in assignment 1, the skeleton can be interacted with using the mouse.

## Assignment 3: Texture Mapping

The focus of assignment 3 is performing different methods of texture mapping to 3D models. Namely: planar, cylindrical and spherical mapping.

The texture mapping method can be changed by pressing specific keys:

* Planar: P
* Cylindrical: C
* Spherical S

Different textures can be used by changing the `Assignment_3/data/texture.png` file.

The basic interactions from assignment 1 are also available here, with the exception of colour changing. Thus, it is possible to rotate the model and observe how the various methods of texture mapping affect the appearance of the model at different angles.

## Assignment 4: Ray Tracing

The final assignment is on ray tracing and lighting. Ray tracing is done against several types of objects - spheres, planes and triangles - and is used for lighting computation. As for lighting, the method that is implemented is Phong lighting.

Unlike the other assignments, there is no user interaction here. Instead, the program produces an image based on the input data. However, the code is not perfect, and there may be inconsistent output despite running the program on the same input multiple times.
