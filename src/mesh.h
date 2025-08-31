#ifndef MESH_H
#define MESH_H


#include "vertex.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <filesystem>

class Mesh {
public:
    Mesh(const std::string& modelPath, const std::string& texturePath);
    ~Mesh();

    const std::vector<Vertex>& getVertices() const {return vertices;}
    const std::vector<uint16_t>& getIndices() const {return indices;}

    int getTexWidth() const  {return texWidth;}
    int getTexHeight() const {return texHeight;}
    int getTexChannels() const {return texChannels;}
    stbi_uc* getPixels() const {return pixels;}

    bool hasTexture() const {return !texturePath.empty() && pixels != nullptr;}

private:
    void loadModel();
    void loadTexture();

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    std::string modelPath;
    std::string texturePath;
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = nullptr;
};

#endif