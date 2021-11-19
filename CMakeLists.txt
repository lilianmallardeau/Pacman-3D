cmake_minimum_required(VERSION 3.20)
project(Pacman_3D)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenGL REQUIRED)
add_subdirectory(lib/glad)
add_subdirectory(lib/glfw)
add_subdirectory(lib/glm)

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIR})
endif()

#set(ASSIMP_LIBRARY_DIRS "lib/assimp/build/code")
#set(ASSIMP_INCLUDE_DIRS "lib/assimp/include" "lib/assimp/build/include")
#include_directories(${ASSIMP_INCLUDE_DIRS})
#link_directories(${ASSIMP_LIBRARY_DIRS})

add_compile_options(-Wall -Wextra)

add_executable(Pacman_3D src/main.cpp src/game/Map.h src/game/Map.cpp src/opengl/BufferLayout.cpp src/opengl/BufferLayout.h src/opengl/IndexBuffer.cpp src/opengl/IndexBuffer.h src/opengl/opengl_abstraction.hpp src/opengl/Program.cpp src/opengl/Program.h src/opengl/Renderer.cpp src/opengl/Renderer.h src/opengl/Shader.cpp src/opengl/Shader.h src/opengl/Texture.cpp src/opengl/Texture.h src/opengl/VertexArray.cpp src/opengl/VertexArray.h src/opengl/VertexBuffer.cpp src/opengl/VertexBuffer.h src/game/Game.cpp src/game/Game.h src/parameters.h src/game/Ghost.cpp src/game/Ghost.h src/utils/Model.cpp src/utils/Model.h src/utils/Mesh.cpp src/utils/Mesh.h src/utils/Object3D.cpp src/utils/Object3D.h)

target_include_directories(Pacman_3D PRIVATE lib/)

target_link_libraries(Pacman_3D
	PRIVATE
	glad
	glfw
	glm
	${ASSIMP_LIBRARIES}
	OpenGL::GL)
