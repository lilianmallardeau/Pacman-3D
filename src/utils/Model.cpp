//
// Created by lilian on 17/11/2021.
//

#include "Model.h"

#include <iostream>

#include <glm/gtc/type_ptr.hpp>


void Model::loadModel(const std::string &file, const glm::mat4& transform) {
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file, aiProcess_Triangulate | aiProcess_OptimizeMeshes);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return;
    }
    processNode(scene->mRootNode, scene, glm_to_assimp_matrix(transform));
}

aiMatrix4x4 Model::glm_to_assimp_matrix(const glm::mat4 &glm_mat) {
    assert(sizeof(glm::mat4) == sizeof(aiMatrix4x4));
    aiMatrix4x4 assimp_mat;
    memcpy(&assimp_mat.a1, glm::value_ptr(glm::transpose(glm_mat)), sizeof(glm::mat4));
    return assimp_mat;
}

void Model::processMesh(aiMesh *mesh, const aiScene *scene, const aiMatrix4x4& transform) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
        Vertex v{};
        if (mesh->HasPositions()) {
            auto transformedVertex = transform * mesh->mVertices[i];
            v.position = {transformedVertex.x, transformedVertex.y, transformedVertex.z};
        }
        if (mesh->HasNormals()) {
            aiMatrix3x3 normalTransform(transform);
            auto transformedNormal = normalTransform.Inverse().Transpose() * mesh->mNormals[i];
            v.normal = {transformedNormal.x, transformedNormal.y, transformedNormal.z};
        }
        if (mesh->HasTextureCoords(0))
            v.texCoords = {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y};
        vertices.push_back(v);
    }
    if (mesh->HasFaces()) {
        for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
            // /!\ should not be more than 3 because of triangulation
            for (unsigned int j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
                indices.push_back(mesh->mFaces[i].mIndices[j]);
            }
        }
    }
    _meshes.emplace_back(vertices, indices);
}

void Model::processNode(aiNode *node, const aiScene *scene, const aiMatrix4x4& transform) {
    auto localTransform = transform * node->mTransformation;
    for (unsigned int i = 0; i < node->mNumMeshes; ++i) {
        processMesh(scene->mMeshes[node->mMeshes[i]], scene, localTransform);
    }
    for (unsigned int i = 0; i < node->mNumChildren; ++i) {
        processNode(node->mChildren[i], scene, localTransform);
    }
}
