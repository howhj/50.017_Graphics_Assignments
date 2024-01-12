 Note: I cleared the build directory so the usual steps for building have to be done.

1. cd [path to build directory]
2. cmake ..
3. make
4. ./Assignment_3

########################################################################

2.1 Planar Parameterization

This part was very straightforward since all the information that was needed was already given in the handout.

########################################################################

2.2 Cylindrical Parameterization

I used the mapping from slide 31 of the texture mapping slides to determine θ and φ. However, I made a couple of mistakes.

For θ, I forgot I was supposed to normalize the values to [0, 1], so I was blindly converting the output of atan2f(y, x) from radians to degrees. After discussing with Keng Hoy, I realised I need to change the range of values from [-π, π] to [0, 1].

For φ, I was under the impression the bounding box was for the camera view i.e. in 2D. So, I thought the height of the bounding box is determined using ymin and ymax like in the planar parameterization. I did not know the bounding box also has values for the z-axis. After experimenting with various parts of my code, I eventually found out about this myself.

########################################################################

2.3 Spherical Parameterization

Like cylindrical parameterization, I relied on the mapping from the texture mapping slides.

Since θ is calculated the same way as in cylindrical parameterization, I faced the same issue here.

φ was harder to solve. Initially, I tried to obtain ρsin(φ) by calculating x/cos(θ), then taking atan2f(ρsin(φ), z). However, this resulted in significant floating point errors that affected the texture mapping. So, I tried another approach where I calculated ρsin(φ) using sqrt(x^2 + y^2). This turned out to be more successful.

However, I was still having issues with the mapping. After checking the values of φ I am getting, I found out that atan2f is only returning values from [0, π]. Thus, I simply have to change how I normalize φ to map the texture correctly.
