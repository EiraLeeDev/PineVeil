#include "mesh.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


Mesh::Mesh(const std::string& modelPath, const std::string& texturePath) : modelPath(modelPath), texturePath(texturePath) {
    loadModel();
    loadTexture();
}

Mesh::~Mesh() {
    if (pixels) {
        stbi_image_free(pixels);
    }
}

void Mesh::loadModel() {
    // Figure put mesh loading tommorrow
}