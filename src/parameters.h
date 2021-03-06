#ifndef PARAMETERS_H
#define PARAMETERS_H

// Full screen mode and window settings
#define FULLSCREEN                  1
#define WINDOW_SIZE                 1000, 700 // Not used if FULLSCREEN is set to 1

// GAME PARAMETERS
#define PACMAN_SPEED                .10f // Unused
#define GHOSTS_SPEED                .02f
#define GHOSTS_NUMBER               5
#define MIN_GHOST_INIT_DISTANCE     10 // A value larger than the map will result in an infinite loop while initializing the ghosts

// MAP STYLE
#define BACKGROUND_COLOR            0, 0, 0
#define WALLS_TEXTURE               "assets/textures/wall.jpg"
#define GROUND_COLOR                0.2f, 0.2f, 0.2f

// MODELS
#define GHOST_MODEL                 "assets/3d_models/kodama/kodama.obj"
#define GHOST_MODEL_SCALING         0.1f
#define GHOST_MODEL_TRANSLATION     0.0f, 0.0f, 0.4f
#define GHOST_MODEL_ROTATION        90.0f, 90.0f, 0.0f // Rotation of the model around the x, y, z axis, in degrees
#define PELLET_MODEL                "assets/3d_models/diamond/diamond.obj"
#define PELLET_MODEL_SCALING        0.1f
#define PELLET_MODEL_TRANSLATION    0.0f, 0.0f, 0.2f
#define PELLET_MODEL_ROTATION       90.0f, 0.0f, 0.0f // Rotation of the model around the x, y, z axis, in degrees

// CAMERA & VIEW
#define FOV                         80.0f
#define ASPECT_RATIO                4.0f / 3.0f

// DEBUG OPTIONS AND STUFF
#define PRINT_FPS                   0
#define ENABLE_SOUND                1


#endif //PARAMETERS_H
