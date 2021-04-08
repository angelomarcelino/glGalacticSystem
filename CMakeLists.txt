cmake_minimum_required(VERSION 3.0)
project(lista_adjacencias C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

add_executable(main src/main.cpp)
target_link_libraries(main ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )