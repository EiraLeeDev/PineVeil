#!/bin/bash

# Change to the build directory and run make
cd /mnt/secondary-drive/Vulkan/PineVeil/build \
&& cmake -DCMAKE_BUILD_TYPE=Release .. \
&& cmake --build . --config Release \
&& ./VulkanProject
