//
// Created by lilian on 18/11/2021.
//

#include <glm/gtx/transform.hpp>
#include "Object3D.h"

void Object3D::updateTransformMatrix() {
    if (transform == nullptr)
        transform = new glm::mat4;
    *transform = translationMatrix * rotationMatrix * scaleMatrix;
}

void Object3D::setTranslation(const glm::vec3 &translationVector, bool updateTransform) {
    translationMatrix = glm::translate(glm::mat4(1.0f), translationVector);
    if (updateTransform) updateTransformMatrix();
}

void Object3D::setScale(const glm::vec3 &scaleVector, bool updateTransform) {
    scaleMatrix = glm::scale(scaleVector);
    if (updateTransform) updateTransformMatrix();
}

void Object3D::setScale(float scaleFactor, bool updateTransform) {
    setScale(glm::vec3(scaleFactor), updateTransform);
}

void Object3D::setRotation(float x_rot, float y_rot, float z_rot, bool updateTransform) {
    rotationMatrix =
            glm::rotate(glm::radians(x_rot), glm::vec3(1, 0, 0)) *
            glm::rotate(glm::radians(y_rot), glm::vec3(0, 1, 0)) *
            glm::rotate(glm::radians(z_rot), glm::vec3(0, 0, 1));
    if (updateTransform) updateTransformMatrix();
}
