//
// Created by lilian on 16/11/2021.
//

#include "Game.h"

#include <algorithm>
#include <iostream>

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "../parameters.h"

#if ENABLE_SOUND == 1
    #include <SFML/Audio.hpp>
#endif


bool Game::init() {
    srand(time(NULL)); //NOLINT(cert-msc51-cpp, modernize-use-nullptr)
    if (!Renderer::init(WINDOW_SIZE, "Pacman 3D", FULLSCREEN))
        return false;
    glEnable(GL_DEPTH_TEST);

    // --------- Face culling --------
//    glEnable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
//    glFrontFace(GL_CW);
    // --------- Face culling --------

    // ------------ Mouse ------------
    glfwSetInputMode(Renderer::context, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(Renderer::context, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    glfwGetCursorPos(Renderer::context, &x_mousePos, &y_mousePos);
    // ------------ Mouse ------------

    // ------------ Sounds ------------
#if ENABLE_SOUND == 1
    startSoundBuffer.loadFromFile("assets/sounds/pacman_beginning.wav");
    chompSoundBuffer.loadFromFile("assets/sounds/pacman_chomp.wav");
    deathSoundBuffer.loadFromFile("assets/sounds/pacman_death.wav");
    startSound.setBuffer(startSoundBuffer);
    chompSound.setBuffer(chompSoundBuffer);
    deathSound.setBuffer(deathSoundBuffer);
#endif
    // ------------ Sounds ------------

    map.init();
    pacmanPosition = {(float) map.getInitialPosition().x, (float) map.getInitialPosition().y, 0.6f};
    previousPacmanPosition = pacmanPosition;
    cameraDirection = glm::vec3(1, 0, 0);

    projectionMatrix = glm::perspective(glm::radians(FOV), ASPECT_RATIO, 1e-4f, 100.0f);

    objectsShader = new Program("src/shaders/objects/vertex.shader", "src/shaders/objects/fragment.shader");

    loadModelsAndTextures();

    ghostTransformMatrices = (glm::mat4*) calloc(GHOSTS_NUMBER, sizeof(glm::mat4));
    for (int i = 0; i < GHOSTS_NUMBER; ++i) {
        Position ghostPos;
        do {
            ghostPos = map.getRandomPosition(MapElement::PELLET);
        } while (ghostPos % map.getInitialPosition() < MIN_GHOST_INIT_DISTANCE);
        Ghost* ghost = new Ghost(ghostPos, ghostModel);
        ghost->obj.setTransformMatrixPointer(&ghostTransformMatrices[i]);
        ghosts.push_back(ghost);
    }

    return true;
}

void Game::loadModelsAndTextures() {
    glm::vec3 rotation_ghosts(GHOST_MODEL_ROTATION);
    glm::vec3 rotation_pellets(PELLET_MODEL_ROTATION);
    glm::mat4 ghostModelTransform =
            glm::translate(glm::vec3{GHOST_MODEL_TRANSLATION}) *
            glm::rotate(glm::radians(rotation_ghosts.x), glm::vec3{1, 0, 0}) *
            glm::rotate(glm::radians(rotation_ghosts.y), glm::vec3{0, 1, 0}) *
            glm::rotate(glm::radians(rotation_ghosts.z), glm::vec3{0, 0, 1}) *
            glm::scale(glm::vec3{GHOST_MODEL_SCALING});
    glm::mat4 pelletModelTransform =
            glm::translate(glm::vec3{PELLET_MODEL_TRANSLATION}) *
            glm::rotate(glm::radians(rotation_pellets.x), glm::vec3{1, 0, 0}) *
            glm::rotate(glm::radians(rotation_pellets.y), glm::vec3{0, 1, 0}) *
            glm::rotate(glm::radians(rotation_pellets.z), glm::vec3{0, 0, 1}) *
            glm::scale(glm::vec3{PELLET_MODEL_SCALING});

    ghostModel = new Model(GHOST_MODEL, ghostModelTransform);
    std::for_each(ghostModel->meshes().cbegin(), ghostModel->meshes().cend(), [](const Mesh& mesh){mesh.setupBuffers();});
    pelletModel = new Model(PELLET_MODEL, pelletModelTransform);
    std::for_each(pelletModel->meshes().cbegin(), pelletModel->meshes().cend(), [](const Mesh& mesh){mesh.setupBuffers();});

    // TODO load textures
    // load textures...
}

void Game::processInput(double deltaTime) {
    float cameraSpeed = 2.5f * (float) deltaTime;
    if (glfwGetKey(Renderer::context, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(Renderer::context, GLFW_TRUE);
    if (glfwGetKey(Renderer::context, GLFW_KEY_W) == GLFW_PRESS) // Move forward
        pacmanPosition += glm::vec3(cameraDirection.x, cameraDirection.y, 0.0f) * cameraSpeed;
    if (glfwGetKey(Renderer::context, GLFW_KEY_S) == GLFW_PRESS) // Move backward
        pacmanPosition -= glm::vec3(cameraDirection.x, cameraDirection.y, 0.0f) * cameraSpeed;
    if (glfwGetKey(Renderer::context, GLFW_KEY_A) == GLFW_PRESS) // Rotate left
        cameraDirection = glm::normalize(glm::rotateZ(cameraDirection, 0.5f * cameraSpeed));
    if (glfwGetKey(Renderer::context, GLFW_KEY_D) == GLFW_PRESS) // Rotate right
        cameraDirection = glm::normalize(glm::rotateZ(cameraDirection, -0.5f * cameraSpeed));
}

void Game::processMouseInput(double deltaTime) {
    double xPos, yPos;
    glfwGetCursorPos(Renderer::context, &xPos, &yPos);
    double xDiff = (x_mousePos - xPos) * deltaTime/8;
    double yDiff = (y_mousePos - yPos) * deltaTime/8;

    cameraDirection = glm::rotate(cameraDirection, (float) xDiff, glm::vec3(0, 0, 1));
    cameraDirection = glm::rotate(cameraDirection, (float) yDiff, glm::cross(cameraDirection, glm::vec3(0, 0, 1)));
    cameraDirection = glm::normalize(cameraDirection);
    x_mousePos = xPos;
    y_mousePos = yPos;
}

void Game::loop() {
    Renderer::setClearColor(BACKGROUND_COLOR);
    double deltaTime = 0.0;	// Time between current frame and last frame
    double lastFrame = 0.0; // Time of last frame

    #if ENABLE_SOUND == 1
    startSound.play();
    #endif

    while (!glfwWindowShouldClose(Renderer::context)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        #if PRINT_FPS == 1
        std::cout << "FPS: " << 1.0f/deltaTime << std::endl;
        #endif

        glfwPollEvents();
        processInput(deltaTime);
        processMouseInput(deltaTime);

        if (!gameOver && !checkForGameEnd()) {
            // Move ghosts
            for (Ghost *ghost: ghosts) {
                if (map.canTurn(ghost->getPosition(), ghost->getDirection())) {
                    Direction newDir;
                    do {
                        newDir = map.getRandomDirection(ghost->getPosition());
                    } while (newDir == Map::getOppositeDirection(ghost->getDirection()));
                    ghost->setDirection(newDir);
                }
                else if (!map.canGo(ghost->getPosition(), ghost->getDirection(), ghost->getDirection()))
                    ghost->setDirection(Map::getOppositeDirection(ghost->getDirection()));
                ghost->move(GHOSTS_SPEED);
            }

            // Pacman
            if (map.removePellet(Position(pacmanPosition.x, pacmanPosition.y))) {
                std::cout << map.count(MapElement::PELLET) << " pellets left!" << std::endl;
                #if ENABLE_SOUND == 1
                chompSound.play();
                #endif
            }

            // Prevent Pacman from going outside tunnels
            if (!map.isTunnel(Position(pacmanPosition.x, pacmanPosition.y)))
                pacmanPosition = previousPacmanPosition;
            else
                previousPacmanPosition = pacmanPosition;

            // Update view matrix
            computeViewMatrix();

            // Rendering everything on the screen
            Renderer::Clear();
            setCommonUniforms(*map.shader);
            renderWalls();
            renderGround();
            renderGhosts();
            renderPellets();

            glfwSwapBuffers(Renderer::context);
        }
        else {
            #if ENABLE_SOUND == 1
            if (deathSound.getStatus() == sf::Sound::Status::Stopped)
            #endif
                glfwSetWindowShouldClose(Renderer::context, GLFW_TRUE);
        }
    }
    glfwTerminate();
}



void Game::computeViewMatrix() {
    viewMatrix = glm::lookAt(pacmanPosition, pacmanPosition + cameraDirection, glm::vec3(0, 0, 1));
}

void Game::setCommonUniforms(Program& shader) {
    shader.setUniformMatrix4("u_viewProjMatrix", projectionMatrix * viewMatrix);
    shader.setUniform<float>("u_pacmanPosition", {pacmanPosition.x, pacmanPosition.y, pacmanPosition.z}, false);
    shader.setUniform<float>("u_lightPosition", {pacmanPosition.x, pacmanPosition.y, pacmanPosition.z}, false);
//    shader.setUniform<float>("u_lightPosition", {(float) map.width / 2.0f, (float) map.height / 2.0f, 5.0f}, false);
}

void Game::renderGround() {
    map.shader->setUniform<int>("u_enableTexture", {0});
    map.shader->setUniform<float>("u_groundColor", {GROUND_COLOR});
    Renderer::Draw(*map.vertexArray, *map.groundIndexBuffer, *map.shader);
}

void Game::renderWalls() {
    map.shader->setUniform<int>("u_enableTexture", {1});
    Renderer::Draw(*map.vertexArray, *map.wallsIndexBuffer, *map.shader);
}

void Game::renderPellets() {
    std::vector<glm::mat4> transformMatrices;
    transformMatrices.reserve(map.count(MapElement::PELLET));
    for (unsigned int x = 0; x < map.width; ++x) {
        for (unsigned int y = 0; y < map.height; ++y) {
            if (map[{x, y}] == MapElement::PELLET) {
                transformMatrices.push_back(glm::translate(glm::mat4(1.0f), {(float) x, (float) y, 0.0f}));
            }
        }
    }
    objectsShader->setUniformMatrix4("u_transformMatrices", transformMatrices.data(), transformMatrices.size());
    objectsShader->setUniform<float>("u_objectColor", {0.1f, 0.4f, 1.0f, 0.7f});
    objectsShader->setUniform<float>("u_ambiantIntensity", {1.8f});
    objectsShader->setUniform<float>("u_diffuseIntensity", {0.0f});
    objectsShader->setUniform<float>("u_specularIntensity", {0.0f});
    for (const auto& mesh : pelletModel->meshes())
        Renderer::DrawInstances(*mesh.vao, *mesh.ibo, *objectsShader, transformMatrices.size());
}

void Game::renderGhosts() {
    setCommonUniforms(*objectsShader);
    objectsShader->setUniformMatrix4("u_transformMatrices", ghostTransformMatrices, GHOSTS_NUMBER);
    objectsShader->setUniform<float>("u_objectColor", {1.0f, 1.0f, 1.0f, 1.0f});
    objectsShader->setUniform<float>("u_ambiantIntensity", {0.1f});
    objectsShader->setUniform<float>("u_diffuseIntensity", {1.0f});
    objectsShader->setUniform<float>("u_specularIntensity", {0.5f});
    for (const auto& mesh : ghostModel->meshes())
        Renderer::DrawInstances(*mesh.vao, *mesh.ibo, *objectsShader, GHOSTS_NUMBER);
}

bool Game::checkForCollision() const {
    return std::any_of(ghosts.cbegin(), ghosts.cend(), [this](Ghost* ghost){
        return ghost->getPosition().round() == Position(pacmanPosition.x, pacmanPosition.y).round();
    });
}

bool Game::checkForSuccess() const {
    return map.count(MapElement::PELLET) == 0;
}

bool Game::checkForGameEnd() {
    if (checkForCollision()) {
        gameOver = true;
        #if ENABLE_SOUND == 1
        if (deathSound.getStatus() != sf::Sound::Status::Playing) deathSound.play();
        #endif
        return true;
    }
    return checkForSuccess();
}
