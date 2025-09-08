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
    std::ifstream infile(modelPath);

    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> colors;

    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;
    std::string line;
    
    while (std::getline(infile, line)) { 
        std::string key = line.substr(0, line.find(" "));

        if (key == "v") {
            std::istringstream iss(line.substr(line.find(" ") + 1));
            float x, y, z;
            iss >> x >> y >> z;
            positions.push_back({x, y, z});
        } else if (key == "vt") {
            std::istringstream iss(line.substr(line.find(" ") + 1));
            float x, y;
            iss >> x >> y;
            texCoords.push_back({x, y});
        } else if (key == "vn") {
            std::istringstream iss(line.substr(line.find(" ") + 1));
            float x, y, z;
            iss >> x >> y >> z;
            colors.push_back({x, y, z});
        } else if (key == "f") {
            std::istringstream iss(line.substr(line.find(" ") + 1));
            std::string token;
            
            while (iss >> token) {
                std::istringstream tokenStream(token);
                std::string part;
                int vertexIndex = -1, texIndex = -1, colorIndex = -1;
                int count = 0;
                
                while (std::getline(tokenStream, part, '/')) {
                    if (!part.empty()) {
                        if (count == 0) vertexIndex = std::stoi(part) - 1; // Convert to 0-based
                        else if (count == 1) texIndex = std::stoi(part) - 1;
                        else if (count == 2) colorIndex = std::stoi(part) - 1;
                    }
                    count++;
                }
                
                Vertex vertex;
                if (vertexIndex >= 0 && vertexIndex < positions.size()) {
                    vertex.pos = positions[vertexIndex];
                }
                if (texIndex >= 0 && texIndex < texCoords.size()) {
                    vertex.texCoord = texCoords[texIndex];
                }
                if (colorIndex >= 0 && colorIndex < colors.size()) {
                    vertex.color = colors[colorIndex];
                }
                
                vertices.push_back(vertex);
                indices.push_back(static_cast<uint16_t>(vertices.size() - 1));
            }
        }
    }

    Mesh::vertices = vertices;
    Mesh::indices = indices;
}

void Mesh::loadTexture() {
    // Figure put mesh loading tommorrow
}