# Earthquake-Visualization

## Description
Earthquakes from the last century are visualized on a map of the earth. When the program starts, a rectangular map of the earth starts, and the timeline is set to about a century ago. Time slowly moves forward, and whenever and earthquake with a magnitude greater than 5.0 happens, a sphere is placed in the location of that earthquake. The sphere size and color is proportional to the magnitude of the earthquake. The bigger and more red the sphere, the larger the earthquake. Can switch to 3D globe mode and speed up / slow down time.

## Tools Used
- OpenGL 2 immediate mode for the drawing
- SDL2 for the windowing and user input

## Features
- Texture mapping of the earth onto the rectangular mesh
- Animating the warping of the map and globe when switching between 2D and 3D views
- Adjustable camera

## Controls
- Spacebar: Stop time
- Left / Right arrows: Slow down / speed up time
- 's': Switch between 2D and 3D views
- Mouse: Click and move camera around

## Installing
On Linux:
```sh
g++ main.cpp -lGL -lGLU -lSDL2 -lGLEW
```
