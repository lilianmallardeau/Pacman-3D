//
// Created by lilian on 16/11/2021.
//

#ifndef GAME_H
#define GAME_H

#include <string>
#include <vector>

#include "../parameters.h"
#include "Map.h"
#include "Ghost.h"

#if ENABLE_SOUND == 1
    #include <SFML/Audio.hpp>
#endif

class Game {
    private:
        Map map;
        glm::vec3 pacmanPosition;
        glm::vec3 previousPacmanPosition;
        glm::vec3 cameraDirection;
        double x_mousePos, y_mousePos;
        glm::mat4 viewMatrix, projectionMatrix;
        std::vector<Ghost*> ghosts;
        glm::mat4 *ghostTransformMatrices;
        bool gameOver = false;

        Model *ghostModel{}, *pelletModel{};

        Program *objectsShader{};

        // Sound
        #if ENABLE_SOUND == 1
        sf::SoundBuffer startSoundBuffer, chompSoundBuffer, deathSoundBuffer;
        sf::Sound startSound, chompSound, deathSound;
        #endif

    public:
        explicit Game(const std::string & level_file) : map(level_file) {};
        void processInput(double deltaTime);
        void processMouseInput(double deltaTime);
        bool init();
        void loop();

        void loadModelsAndTextures();

        void computeViewMatrix();
        void setCommonUniforms(Program& shader);

        void renderGround();
        void renderWalls();
        void renderPellets();
        void renderGhosts();

        [[nodiscard]] bool checkForCollision() const;
        [[nodiscard]] bool checkForSuccess() const;
        [[nodiscard]] bool checkForGameEnd();

};


#endif //GAME_H
