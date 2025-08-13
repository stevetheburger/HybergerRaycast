# HybergerRaycast

Credit to 3DSage (https://www.youtube.com/@3DSage) for providing the instruction and raycast algorithms necessary for this project to work.
Raycast tutorial: https://www.youtube.com/watch?v=gYRrGTC7GtA&t=17s

Overview:
 This program was developed to embed SDL instead of using OpenGL directly with the dll as in the tutorial.

Detail:
  Was developed to use an SDL layer in embedded form inside a standalone executable as opposed to using an OpenGL dll directly and as a separate file as was specied in the tutorial. Was developed as part of a game engine project, attempting to create an engine for a high fantasy RPG loosely inspired by Elder Scrolls I: Arena. This project was finished and abandoned for a short misadventure into Vulkan with C bindings, which itself is not preserved and didn't have much to show when compared to this. This project was developed with software rendering in mind, with hardware rendering to be added later in tandem, not knowing at the inception of the project that on modern systems this is really just not done and might be impossible.
  Includes vertical/horizontal shading to add depth, as well as different colors, green, blue, and red to differentiate different types of walls. Textures were planned as the next feature to implement as well as collisions. Blue walls were intended to function as doors.

Running:
1. Open the Visual Studio solution and build it, recommended in Release|x64, although it should also work in Debug and x86.
2. Once it is finished building, which shouldn't take long at all. Open the x64/Release folder, and run HybergerRaycast.exe.
3. Contols are WASD with Q and E to turn. Blue walls are intended as doors.
5. Walls do not colide in my program, so you can just pass through them, though fair bit of warning wandering too far away from the designed rooms can cause undefined behavior.
6. Use the exit button on the top right to stop the program when satisfied.
