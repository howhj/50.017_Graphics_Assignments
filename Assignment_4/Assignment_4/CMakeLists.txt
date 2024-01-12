cmake_minimum_required(VERSION 2.8)
project(Assignment_4)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")
set(CMAKE_CXX_STANDARD 11)

add_definitions(-Wno-deprecated-declarations)

# OPENGL
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")


# GLUT
find_package(GLUT REQUIRED)
include_directories( ${GLUT_INCLUDE_DIR} )
link_directories( ${GLUT_LIBRARY_DIR} )

#source files
file( GLOB SRCFILES 
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h
		${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp)

add_library(source_lib STATIC ${SRCFILES})

include_directories(${CMAKE_SOURCE_DIR}/includes)

# Add OpenGL and GLUT
target_include_directories(source_lib PUBLIC SYSTEM)
target_link_libraries(source_lib ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})


add_executable(Assignment_4 src/main.cpp)
target_link_libraries(Assignment_4 PUBLIC source_lib)

