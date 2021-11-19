//
// Created by lilian on 18/11/2021.
//

#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "Model.h"


class Object3D {
    public:
        Model *model;
        glm::mat4 *transform;
        glm::mat4 scaleMatrix{1.0f};
        glm::mat4 rotationMatrix{1.0f};
        glm::mat4 translationMatrix{1.0f};

    public:
        explicit Object3D(Model *model, glm::mat4* transform = nullptr) : model(model), transform(transform) {};
        void setTransformMatrixPointer(glm::mat4* transform) { this->transform = transform; };
        void updateTransformMatrix();

        void setTranslation(const glm::vec3& translationVector, bool updateTransform = true);
        void setScale(const glm::vec3& scaleVector, bool updateTransform = true);
        void setScale(float scaleFactor, bool updateTransform = true);
        void setRotation(float x, float y, float z, bool updateTransform = true);

};


#endif //OBJECT3D_H
