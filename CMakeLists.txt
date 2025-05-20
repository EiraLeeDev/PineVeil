cmake_minimum_required(VERSION 3.16)
project(VulkanProject VERSION 0.1.0 LANGUAGES CXX)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_TESTS "Build tests" OFF)
option(ENABLE_VALIDATION_LAYERS "Enable Vulkan validation layers" ON)

# Platform-specific configuration
if(WIN32)
    add_definitions(-DPLATFORM_WINDOWS)
    
    # Help CMake find Vulkan on Windows if environment variable is set
    if(DEFINED ENV{VULKAN_SDK})
        set(Vulkan_INCLUDE_DIR "$ENV{VULKAN_SDK}/Include")
        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(Vulkan_LIBRARY "$ENV{VULKAN_SDK}/Lib/vulkan-1.lib")
        else()
            set(Vulkan_LIBRARY "$ENV{VULKAN_SDK}/Lib32/vulkan-1.lib")
        endif()
    endif()
    
    # Set GLFW directory if needed
    if(EXISTS "C:/Users/jackt_gssjmy/glfw")
        set(glfw3_DIR "C:/Users/jackt_gssjmy/glfw/lib/cmake/glfw3")
        # If the above doesn't work, try the alternative:
        # set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Users/jackt_gssjmy/glfw")
    endif()
    
    # Set GLM directory if needed - adjust path as necessary
    if(EXISTS "C:/Users/jackt_gssjmy/glm")
        set(GLM_INCLUDE_DIR "C:/Users/jackt_gssjmy/glm")
    endif()
elseif(UNIX AND NOT APPLE)
    add_definitions(-DPLATFORM_LINUX)
endif()

# Find packages
find_package(Vulkan REQUIRED)
message(STATUS "Found Vulkan: ${Vulkan_LIBRARY}")

# Try to find GLFW
find_package(glfw3 QUIET)
if(NOT glfw3_FOUND)
    message(STATUS "GLFW not found through find_package, attempting direct inclusion")
    # Try direct inclusion if find_package fails
    if(EXISTS "C:/Users/jackt_gssjmy/glfw")
        set(GLFW_INCLUDE_DIR "C:/Users/jackt_gssjmy/glfw/include")
        if(EXISTS "C:/Users/jackt_gssjmy/glfw/lib/glfw3.lib")
            set(GLFW_LIBRARY "C:/Users/jackt_gssjmy/glfw/lib/glfw3.lib")
        elseif(EXISTS "C:/Users/jackt_gssjmy/glfw/lib/libglfw3.a")
            set(GLFW_LIBRARY "C:/Users/jackt_gssjmy/glfw/lib/libglfw3.a")
        elseif(EXISTS "C:/Users/jackt_gssjmy/glfw/lib/glfw3dll.lib")
            set(GLFW_LIBRARY "C:/Users/jackt_gssjmy/glfw/lib/glfw3dll.lib")
        endif()
        
        if(DEFINED GLFW_LIBRARY)
            add_library(glfw INTERFACE)
            target_include_directories(glfw INTERFACE ${GLFW_INCLUDE_DIR})
            target_link_libraries(glfw INTERFACE ${GLFW_LIBRARY})
            message(STATUS "Using GLFW from: ${GLFW_INCLUDE_DIR}")
        else()
            message(FATAL_ERROR "GLFW library files not found in C:/Users/jackt_gssjmy/glfw/lib")
        endif()
    else()
        message(FATAL_ERROR "GLFW not found. Please install GLFW or specify its location.")
    endif()
else()
    message(STATUS "Found GLFW: ${glfw3_DIR}")
endif()

# GLM handling
find_package(glm QUIET)
if(NOT glm_FOUND)
    message(STATUS "GLM not found through find_package, attempting direct inclusion")
    if(EXISTS "C:/Users/jackt_gssjmy/glm")
        set(GLM_INCLUDE_DIRS "C:/Users/jackt_gssjmy/glm")
        message(STATUS "Using GLM from: ${GLM_INCLUDE_DIRS}")
    else()
        message(FATAL_ERROR "GLM not found. Please install GLM or specify its location.")
    endif()
endif()

# Validation layers config
if(ENABLE_VALIDATION_LAYERS)
    add_definitions(-DENABLE_VALIDATION_LAYERS)
endif()

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Vulkan_INCLUDE_DIRS}
)

# Add GLM include if needed
if(DEFINED GLM_INCLUDE_DIRS)
    include_directories(${GLM_INCLUDE_DIRS})
endif()

# Add shader compilation
file(GLOB_RECURSE SHADER_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/assets/shaders/*.frag"
)

# Function to compile shaders
function(compile_shaders)
    # Check if glslc is available
    if(NOT Vulkan_GLSLC_EXECUTABLE)
        if(DEFINED ENV{VULKAN_SDK})
            if(WIN32)
                set(Vulkan_GLSLC_EXECUTABLE "$ENV{VULKAN_SDK}/Bin/glslc.exe")
            else()
                set(Vulkan_GLSLC_EXECUTABLE "$ENV{VULKAN_SDK}/bin/glslc")
            endif()
        endif()
    endif()
    
    if(NOT Vulkan_GLSLC_EXECUTABLE)
        message(WARNING "glslc not found, shader compilation disabled")
        return()
    endif()
    
    message(STATUS "Using glslc: ${Vulkan_GLSLC_EXECUTABLE}")
    
    set(SPIRV_BINARY_FILES "")
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
    
    if(SPIRV_BINARY_FILES)
        add_custom_target(
            Shaders 
            DEPENDS ${SPIRV_BINARY_FILES}
        )
    endif()
endfunction()

compile_shaders()

# Sources
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Add dependency on shaders if available
if(TARGET Shaders)
    add_dependencies(${PROJECT_NAME} Shaders)
endif()

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