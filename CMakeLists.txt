cmake_minimum_required(VERSION 3.16)
project(VulkanProject VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_VALIDATION_LAYERS "Enable Vulkan validation layers" ON)

# Find packages
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)

# Define platform-specific variables
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Validation layers config
if(ENABLE_VALIDATION_LAYERS)
    add_definitions(-DENABLE_VALIDATION_LAYERS)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Vulkan_INCLUDE_DIRS}
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
)

# Add shader compilation
file(GLOB_RECURSE SHADER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.frag"
)

# Function to compile shaders
function(compile_shaders)
    foreach(SHADER ${SHADER_SOURCES})
        get_filename_component(FILENAME ${SHADER} NAME)
        get_filename_component(DIR ${SHADER} DIRECTORY)
        get_filename_component(EXTENSION ${SHADER} EXT)
        
        set(SPIRV "${DIR}/${FILENAME}.spv")
        
        add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} -o ${SPIRV} ${SHADER}
            DEPENDS ${SHADER}
            COMMENT "Compiling shader ${FILENAME}"
        )
        list(APPEND SPIRV_BINARY_FILES ${SPIRV})
    endforeach()
    
    add_custom_target(
        Shaders 
        DEPENDS ${SPIRV_BINARY_FILES}
    )
endfunction()

compile_shaders()

# Sources
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} Shaders)

# Link libraries
target_link_libraries(${PROJECT_NAME}
    Vulkan::Vulkan
    glfw
)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# Install rules
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/"
        DESTINATION "assets/shaders"
        FILES_MATCHING PATTERN "*.spv")
