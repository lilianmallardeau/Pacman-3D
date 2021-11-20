# Pacman 3D
3D version of my [Homemade Pacman](https://github.com/lilianmallardeau/Homemade-Pacman), made with C++ and OpenGL

![pacman_3d_screenshot](https://user-images.githubusercontent.com/56023249/142716704-84f62c4b-0dcc-4153-b0a3-1b204935ffd1.png)


## Dependencies
[ASSIMP](https://github.com/assimp/assimp) (to load the 3D models) and [SFML](https://github.com/SFML/SFML) (to play sounds) are needed to compile the program. For Debian based distributions, they can be installed with this command:
```
sudo apt install libassimp-dev libsfml-dev
```

## Clone

Clone the repository with the `recurse-submodules` option to clone the libraries repositories as well: 
```
git clone --recurse-submodules https://github.com/lilianmallardeau/Pacman-3D.git
```
Then move to the cloned folder.

## Build and run
```
mkdir build
cd build
cmake ..
make
cd ..
./build/Pacman_3D levels/level0
```
To run, the current working directory **MUST** be the root folder of the repository, because the program has to load the shaders files in the `src/shaders/` folder and the textures, 3D models and sounds in the `assets/` folder.

## How to play
The goal is to collect all the pellets (diamonds) without being eaten by a ghost.
You can move forward/backward with the W/S keys (Z/S for AZERTY keyboards), and rotate the camera with the A/D keys (Q/D for AZERTY keyboards).
The mouse can also be used to move the camera.

## Changing the game options
Some game options can be changed in the `src/parameters.h` header file. Although their name is already kind of explicit, here is a brief description of each of them:

| Parameter | Description |
|:---------:|:-----------:|
| `FULLSCREEN` | 1 to enable fullscreen, 0 otherwise |
| `WINDOW_SIZE` | Size of the window if fullscreen is disabled. Ignored if `FULSCREEN` is set to 1. |
| `PACMAN_SPEED` | Pacman speed  |
| `GHOSTS_SPEED` | Ghosts speed |
| `GHOSTS_NUMBER` | Number of ghosts |
| `MIN_GHOST_INIT_DISTANCE` | Minimum distance between Pacman and the generated ghosts |
| `BACKGROUND_COLOR` | Background color |
| `WALLS_TEXTURE` | Texture for the walls |
| `GROUND_COLOR` | Color of the ground |
| `{GHOST,PELLET}_MODEL` | 3D model for the ghosts/pellets |
| `{GHOST,PELLET}_MODEL_SCALING` | Scaling applied when loading ghost/pellet 3D model. Can be a scalar (uniform scaling) or a vector of 3 components |
| `{GHOST,PELLET}_MODEL_TRANSLATION` | Translation applied when loading the ghost/pellet 3D model |
| `{GHOST,PELLET}_MODEL_ROTATION` | Rotation applied when loading the ghost/pellet 3D model |
| `FOV` | Field of view of the camera (degrees) |
| `ASPECT_RATIO` | Aspect ratio of the window / screen |
| `PRINT_FPS` | 1 to enable printing the number of processed frames per second at each frame, 0 to disable |
| `ENABLE_SOUND` | 1 to enable sounds, 0 to disable |