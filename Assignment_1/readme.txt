Note: I cleared the build directory so the usual steps for building have to be done.

1. cd [path to build directory]
2. cmake ..
3. make
4. ./Assignment_1

########################################################################

1. Mesh Loading

This was easily the most difficult part. I faced two main issues, parsing the OBJ file and inserting the normal vectors into verList.

I spent the entire week trying to do this on my own, using ifstream and istringstream to read the file line by line. The idea was to look at the line header (v, vn or f), then decide what to do next.

For v, I would insert the data directly into verList, adding 3 more placeholder values to add the corresponding normal in later. Originally, I used NULL for the placeholder, but it turns out it is just 0 in C++ anyway, so I ended up using 0.0f.

For vn, I temporarily stored the data in a separate vector, as I cannot pair the normals to the vertices without the face data.

For f, this is where it got tricky. I would push (vertex index - 1) into triList first. Then, I would use the vertex index to access verList, check if the normal data is all 0.0f, and if so, access the temporary normal vector to grab the corresponding normal data.

At least, that was the idea. I had trouble parsing the face data, as it seems like istringstream did not write anything into f1, f2 and f3. After much debugging and internet searching, I realised it was because I did not know the implications of using the >> operator to write data from a stream object into my variables. Specifically, I did so in my if statements. As such, after my first if statement (for v and vn), there would be nothing left to parse when it got to my second if statement (for f), thus it outputs nothing into f1, f2 and f3.

To solve this, I simply had to create a new istringstream and use that for my second if statement (although I suppose this is very unoptimal).

With that, I could load the OBJ files, albeit inconsistently. The same file may load at first, then when I rerun the program and try to load it again, it may not load. Furthermore, I had another unknown bug when parsing the data, as the models were completely messed up. This was evident with garg.obj and mickey.obj.

In hindsight, this may be because of me checking if the normal data in verList is 0.0f, when I can just overwrite the data over and over, which would better guarantee that I have the correct set of normal data in verList.

Anyway, from here, I went to find out more about how OpenGL loads OBJ files, and in so doing found a tutorial with sample code (https://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/). So, I chose to adapt that code instead, since it looked more optimal (using fscanf to parse the data instead of a lot of istringstreams) and cleaner (much less boilerplate code and fscanf is easier to understand).

Of course, since the sample code did not exactly do what the assignment requires, I tweaked it by removing variables I did not need (like the temporary_vertices vector) and adapting the code from my original attempt for inserting the normal data into verList.

Now, I am able to load the models properly without any issues.

As for remaining bugs, they mainly involve the user interaction part. For example, when I run the executable from my CLI, the OpenGL window opens on top of the CLI, requiring the user to switch back to the CLI window to input the file name. I tried to look up the issue, but could not find any solutions, and this behaviour seems to be affected by one's desktop environment. I am using KDE on Linux, and by changing the settings, I could keep the CLI on top. As such, I'm not sure how to go about tackling this issue.

Also, perhaps this is also another desktop environment issue, but if I were to run the executable by clicking on it, it does not launch the CLI, so I have no way to input the filepath and OpenGL will be stuck.

########################################################################

3. Mesh Coloring and 4. Mesh Transformation

I was originally at a loss of what to do, so I looked up more information about how to do these operations on the internet. From there, I read a tutorial (https://learnopengl.com/Getting-started/Transformations) where I discovered that the glm library already has functions that do the heavy lifting.

I also learned that the glm variables like modelMatrix are actually global, so I can access them within the functions I had to write.
