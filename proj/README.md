# CSCI 4229 Final Project

Neighborhood Driving Simulator

## Features (so far)
  * Procedure for chunk generation, based on spatial grid coordinates (gX and gZ)
  * Randomized house colors, locations, sizes, and appearances
  * Driving through houses (no damage, just didn't bound the user location to roads)
  * Pyramids sometimes appear instead of houses
  * Day and Night mode, skybox and lighting updates to reflect these changes

## Controls
  * W - accelerate forwards
  * S - accelerate backwards
  * A - steer left
  * D - steer right
  * UpArrow - look up
  * DownArrow - look down
  * 0 - return to origin
  * SpaceBar - stop (emergency brake)
  * PageUp - zoom in (decrease FOV)
  * PageDown - zoom out (increase FOV)
  * [, ] - lower, raise view height (respectively)
  * -, + - decrease, increase view dist (respectively)
  * n/N - switch between day and night mode (headlights turn on automatically)

## Things I'm Proud Of
 * Car Movement, it's really silly but it took me embarassingly long to get the camera rotating around with the arrow keys and steering correctly (I ended up sketching it and realizing I was applying transformations out of order)
 * Car Headlights, we've already covered regular light sources such as the sun, but it was really fun to work with a spotlight, and play with the settings for a bit.
 * Randomized Terrain, even if it's lacking in diversity, the entire time I was thinking about how games (such as Minecraft) are able to seed random number generators to procedurally generate chunks, and I think it's very cool. I will definitely be spending more time looking into this on my own (although using a noise library rather than rand()).

## Code Reuse
 * framework.h:
   * check(const char *) and fatal(const char *), reused from example code
   * reverseBytes(void *, const int), reused from example code
   * loadTextureFromBMP(const char *), reused from example code
 * shapes.h:
   * drawSkybox(vertex_t *, float, color_t), repurposed from example code (I modified it slightly, ended up disabling the skybox textures (was going to replace the ones from the F-16 example but didn't)
   * drawSun(vertex_t *, float, float, float, int), reused from HW#3 (which I originally reused from example code)

## Improvements for the Future
 * save and load grids (text file output?) instead of having to recalculate vertices every time
 * better driving controls / HUD
 * more lighting modes (sunrise / sunset)
 * shadows / shaders

## Author

William Abrams, William.Abrams@colorado.edu
