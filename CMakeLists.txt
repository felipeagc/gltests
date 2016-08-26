cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_STANDARD 11)

project(gltests)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${gltests_SOURCE_DIR}/cmake")

# Set game sources here
file(GLOB SOURCES src/*.cpp)

add_executable(gltests ${SOURCES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(OpenGL REQUIRED)
if (OPENGL_FOUND)
	include_directories(${OPENGL_INCLUDE_DIR})
	target_link_libraries(gltests ${OPENGL_LIBRARIES})
	target_link_libraries(gltests m)
endif()

find_package(GLEW REQUIRED)
if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	target_link_libraries(gltests ${GLEW_LIBRARIES})
endif()

find_package(GLFW REQUIRED)
if (GLFW_FOUND)
	include_directories(${GLFW_INCLUDE_DIR})
	target_link_libraries(gltests ${GLFW_LIBRARY})
endif()

find_package(ASSIMP REQUIRED)
if(ASSIMP_FOUND)
	include_directories(${ASSIMP_INCLUDE_DIRS})
	target_link_libraries(gltests ${ASSIMP_LIBRARIES})
endif()
