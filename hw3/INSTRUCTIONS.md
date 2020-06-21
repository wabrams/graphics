Write an OpenGL program that incorporates lighting and textures to create a a scene consisting of solid objects that can be viewed in three dimensions under user control.

Use of GLU and GLUT objects or imported objects are prohibited to give you experience with determining normal vectors which are critical in performing the lighting calculations.  Also GLUT objects do not support textures.

The light should move to demonstrate changes to the scene as a result of different light positions.  It will be useful for both debugging and grading if the light can be stopped and moved under user control in addition to moving automatically.  Choose your light positions such that the lighting effects are demonstrated well.

The textures should be used to add realism to the scene.  Choose your textures carefully considering the scale at which you will apply them.  The textures should complement the scene.  All objects in the scene should be textured unless there is a very good reason not to apply textures to some objects.

Use textures that are sized as a power of two, even though it may not be required, because sometimes it makes a difference.  Also keep your textures small.  Textures greater than 512x512 are rarely justified.

What to turn in:
1) Source code, textures, makefile and readme;
2) Approximate time spent on the project.

Hint:  Trust your instincts.  If the lighting doesn't look right, it porbably is not.  A common error is incorrect normals.
