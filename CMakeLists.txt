cmake_minimum_required(VERSION 3.10)
project(NodeEditor)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define project directories
set(PROJECT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(SRC_DIR ${PROJECT_ROOT}/src)
set(EXTERNAL_DIR ${PROJECT_ROOT}/external)

# Paths for ImGui and its backends.
set(IMGUI_DIR ${EXTERNAL_DIR}/imgui)
set(IMGUI_BACKEND_DIR ${IMGUI_DIR}/backends)

# Path for ImGui Node Editor.
set(IMGUI_NODE_EDITOR_DIR ${EXTERNAL_DIR}/ImNodes)

# Path for ImGui Node Editor.
set(IMGUI_FILE_DIALOG_DIR ${EXTERNAL_DIR}/ImFileDialog)

# Path for ImGui Node Editor.
set(STB_DIR ${EXTERNAL_DIR}/stb)

# Add ImGui source files.
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_glfw.cpp
    ${IMGUI_BACKEND_DIR}/imgui_impl_opengl3.cpp
)

# Add ImGui Node Editor source files.
file(GLOB IMGUI_NODE_EDITOR_SOURCES
    ${IMGUI_NODE_EDITOR_DIR}/*.cpp
)

# Add ImGui Node Editor source files.
file(GLOB IMGUI_FILE_DIALOG_SOURCES
    ${IMGUI_FILE_DIALOG_DIR}/*.cpp
)

# Find Vulkan and GLFW.
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# Add all our source files from the src directory.
file(GLOB SRC_FILES ${SRC_DIR}/*.cpp)

# Create the executable.
add_executable(${PROJECT_NAME} ${SRC_FILES} ${IMGUI_SOURCES} ${IMGUI_NODE_EDITOR_SOURCES} ${IMGUI_FILE_DIALOG_SOURCES})

# Include directories for the project and external libraries.
target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_ROOT}
    ${SRC_DIR}
    ${IMGUI_DIR}
    ${IMGUI_BACKEND_DIR}
    ${IMGUI_NODE_EDITOR_DIR}
    ${IMGUI_FILE_DIALOG_DIR}
    ${STB_DIR}
)

# Define compile definitions.
#target_compile_definitions(${PROJECT_NAME} PRIVATE)

# Link required libraries.
target_link_libraries(${PROJECT_NAME} PRIVATE ${OPENGL_LIBRARIES} glfw ${OpenCV_LIBS})
