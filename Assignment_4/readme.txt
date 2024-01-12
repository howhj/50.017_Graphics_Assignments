Note: I cleared the build directory so the usual steps for building have to be done.

1. cd [path to build directory]
2. cmake ..
3. make
4. ./Assignment_4 [...]

########################################################################

3.1 Ray Intersection

The general idea for Sphere, Plane and Triangle is to rearrange the equations in the notes in order to make t (as well as beta and gamma in the case of Triangle) the subject of the equation, in order to solve for it.

Updating the attributes of the Hit object is more involved. I had to look up what other objects can do (like Ray, Hit and Object3D), and also be aware of what attributes are available in Sphere, Plane and Triangle.

For example, in the case of Plane, I had an issue where scene 1 would have an inverted background. It turns out that the issue was due to the d attribute being set to -d already. I was subtracting it again in my equation, not knowing d is already negated.

Another case was in Triangle. Even after finding t and calculating the normal, the object did not show up in my rendered images. Then, I noticed that there was the texCoords attribute that I had not used, and realised that I had to also update which part of the texture to get the colour from.

Transform was also another tricky part for me. After getting a ray intersection, I forgot to transform the normal from local space to world space, which resulted in the wrong colours being used to render the images.

########################################################################

3.2 Lighting Computations

Lighting is a lot more straightforward, as it is a matter of following the equations in the notes. The only part I had trouble with was figuring out how to use the texture information from Triangle, so that I could render triangle meshes. This was solved by checking what the Texture class can do.

########################################################################

There is one more problem that I faced, but I am unable to diagnose where it came from, so I put it in its own section.

The results were very inconsistent. I can run the exact same command multiple times, without recompiling or changing the scene to be rendered, and get different results. For example, the first scene could end up looking like it had a green filter in front of it.

After trying to debug this for a long time but to no avail, I started to wonder if it was a problem with my own computer and not the code, so I employed Keng Hoy's help to test my code on his hardware. While most of the scenes did not have changing results, the cube scene was still inconsistent, so I think there is still a code issue somewhere.

That said, it was possible to get the correct results after running the same command multiple times, so the screenshot folder contains the renders that came out properly.
