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

    map.init();
    pacmanPosition = {(float) map.getInitialPosition().x, (float) map.getInitialPosition().y, 0.6f};
    previousPacmanPosition = pacmanPosition;
    cameraDirection = glm::vec3(1, 0, 0);

    projectionMatrix = glm::perspective(glm::radians(FOV), ASPECT_RATIO, 0.1f, 100.0f);

    obj_shader = new Program("src/shaders/objects/vertex.shader", "src/shaders/objects/fragment.shader");

    loadModelsAndTextures();

    ghostTransformMatrices = (glm::mat4*) calloc(GHOSTS_NUMBER, sizeof(glm::mat4));
    for (int i = 0; i < GHOSTS_NUMBER; ++i) {
        Position ghostPos;
        do {
            ghostPos = map.getRandomPosition(MapElement::PELLET);
        } while (ghostPos % map.getInitialPosition() < MIN_GHOST_INIT_DISTANCE);
        Ghost* ghost = new Ghost(ghostPos, ghost_model);
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

    ghost_model = new Model(GHOST_MODEL, ghostModelTransform);
    std::for_each(ghost_model->meshes().cbegin(), ghost_model->meshes().cend(), [](const Mesh& mesh){mesh.setupBuffers();});
    pellet_model = new Model(PELLET_MODEL, pelletModelTransform);
    std::for_each(pellet_model->meshes().cbegin(), pellet_model->meshes().cend(), [](const Mesh& mesh){mesh.setupBuffers();});

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

    while (!glfwWindowShouldClose(Renderer::context)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        processInput(deltaTime);
        processMouseInput(deltaTime);

        if (!checkForGameEnd()) {
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
            if (map.removePellet(Position(pacmanPosition.x, pacmanPosition.y)))
                std::cout << map.count(MapElement::PELLET) << " pellets left!" << std::endl;

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
    obj_shader->setUniformMatrix4("u_transformMatrices", transformMatrices.data(), transformMatrices.size());
    obj_shader->setUniform<float>("u_Color", {0.1f, 0.3f, 1.0f, 0.7f});
    for (const auto& mesh : pellet_model->meshes())
        Renderer::DrawInstances(*mesh.vao, *mesh.ibo, *obj_shader, transformMatrices.size());
}

void Game::renderGhosts() {
    setCommonUniforms(*obj_shader);
    obj_shader->setUniformMatrix4("u_transformMatrices", ghostTransformMatrices, GHOSTS_NUMBER);
    obj_shader->setUniform<float>("u_Color", {0.0f, 1.0f, 0.0f, 0.8f});
    for (const auto& mesh : ghost_model->meshes())
        Renderer::DrawInstances(*mesh.vao, *mesh.ibo, *obj_shader, GHOSTS_NUMBER);
}

bool Game::checkForCollision() const {
    return std::any_of(ghosts.cbegin(), ghosts.cend(), [this](Ghost* ghost){
        return ghost->getPosition().round() == Position(pacmanPosition.x, pacmanPosition.y).round();
    });
}

bool Game::checkForSuccess() const {
    return map.count(MapElement::PELLET) == 0;
}

bool Game::checkForGameEnd() const {
    return checkForCollision() || checkForSuccess();
}
