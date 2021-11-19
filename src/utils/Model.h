//
// Created by lilian on 17/11/2021.
//

#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"


class Model {
    private:
        std::vector<Mesh> _meshes;

        void loadModel(const std::string& file, const glm::mat4& transform = glm::mat4{1.0f});
        void processMesh(aiMesh *mesh, const aiScene *scene, const aiMatrix4x4& transform = aiMatrix4x4());
        void processNode(aiNode *node, const aiScene *scene, const aiMatrix4x4& transform = aiMatrix4x4());

    public:
        Model(const std::string& file, const glm::mat4& transform = glm::mat4{1.0f}) { loadModel(file, transform); };
        const std::vector<Mesh>& meshes() { return _meshes; };

        static aiMatrix4x4 glm_to_assimp_matrix(const glm::mat4& glm_mat);
};


#endif //MODEL_H
