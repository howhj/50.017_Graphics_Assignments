Note: I cleared the build directory so the usual steps for building have to be done.

1. cd [path to build directory]
2. cmake ..
3. make
4. ./Assignment_2

########################################################################

3.1 Load Skeleton File

This part was relatively easy, as it was mainly just parsing the file, then setting the children and transform for each Joint object. I used what I learned in assignment 1 to help me with this.

The only tricky part was having to transpose the transform matrix. I was helping Keng Hoy debug his code since at the time, we were unaware of this issue. In the end, he was the one who noticed that the transform matrices are transposed in main.cpp and informed me about it.

########################################################################

3.2 Draw Skeleton

Drawing the joints was straighforward. However, I had a lot of trouble with the bones.

Originally, the translation caused the bones to be displaced far away from the joints, the scaling was highly disproportional, and the rotation was not only incorrect but also caused shearing.

It turns out that this was because on the scaling step, I forgot that each child joint's transform is already in the parent joint's space (since this is about hierarchical modeling after all), so I was computing the parentOffset vector as the difference between the child joint's translation and the parent joint's translation.

After discussing with Keng Hoy a bit, he suggested a few possible mistakes I could have made, and this was one of the ones he suggested. Fixing this resulted in the bones being drawn correctly.

For the logic to draw the bones, I learned from my mistakes in assignment 1 and made sure each transformation was first done with an identity matrix and the transformation vector, before multiplying it to the boneMatrix in the correct order.

For the rotation, since the x- and y-axes are arbitrary, I used an alternative method than the one in the handout. I computed the angle between the current z-axis (which is just [0, 0, 1]) and the line between the parent and child joints. Then, I calculated an orthogonal vector using the cross product of the above two vectors, which would serve as the axis of rotation. Finally, I rotated the bone along the orthogonal axis by the angle calculated.

########################################################################

3.3 Change Pose of Skeleton

This part was also relatively easy. Since the function takes in three angles (one per axis), I chose to manually construct a rotation matrix based on slide 40 of the transformation lecture.

I'm not sure based on the wording on the handout if the rotation should be applied to the current rotation of the joint, or to set it as the values entered by the user. I elected to do the latter, so I simply copied the translation vector (the rightmost column) from the joint's original transform into the rotation matrix, then made the new rotation matrix the joint's transform.
