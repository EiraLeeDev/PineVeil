#ifndef MESH_H
#define MESH_H


#include "vertex.h"
#include "stb_image.h"

#include <string>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <filesystem>

class PineVeil;

class Mesh {
public:
    Mesh(const std::string& modelPath, const std::string& texturePath, PineVeil* app);
    ~Mesh();

    void createTextureImage();
    void createTextureImageView();
    void createTextureSampler();
    void createDescriptorSets(VkDescriptorPool descriptorPool, VkDescriptorSetLayout descriptorSetLayout, const std::vector<VkBuffer>& uniformBuffers, VkDeviceSize bufferSize);

    const std::vector<Vertex>& getVertices() const {return vertices;}
    const std::vector<uint16_t>& getIndices() const {return indices;}
    VkBuffer getVertexBuffer() const {return vertexBuffer;}
    VkBuffer getIndexBuffer() const {return indexBuffer;}
    std::vector<VkDescriptorSet> getDescriptorSets() const {return descriptorSets;}

    void cleanup(VkDevice device);

    std::string modelPath;
    std::string texturePath;
    std::vector<Vertex> vertices;
    std::vector<uint16_t> indices;

    VkBuffer vertexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkBuffer indexBuffer;
    VkDeviceMemory indexBufferMemory;

    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;

    std::vector<VkDescriptorSet> descriptorSets;

private:
    PineVeil* app;
    
    int texWidth, texHeight, texChannels;
    stbi_uc* pixels = nullptr;

    void loadModel();
    void createVertexBuffer();
    void createIndexBuffer();
};

#endif