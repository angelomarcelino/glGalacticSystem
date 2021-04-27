cmake_minimum_required(VERSION 3.0)
project(lista_adjacencias C CXX)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )

set(HEADERS
    include/StarSystem.h
    include/Camera.h 
)

set(SOURCES
    src/StarSystem.cpp
    src/Camera.cpp
)

set(PLANETARY_SYS_HEADERS
    include/SysCamera.h
    include/Planet.h
    include/PlanetarySystem.h
    include/Sphere.h
)

set(PLANETARY_SYS_SOURCES
    src/SysCamera.cpp
    src/Planet.cpp
    src/PlanetarySystem.cpp
    src/Sphere.cpp
)

add_executable(main src/main.cpp ${HEADERS} ${SOURCES})
target_link_libraries(main m ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(sys_view tests/SysView.cpp)
target_link_libraries(sys_view ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )

add_executable(planetary_sys src/main_ps.cpp ${PLANETARY_SYS_HEADERS} ${PLANETARY_SYS_SOURCES})
target_link_libraries(planetary_sys m ${OPENGL_LIBRARIES} ${GLUT_LIBRARY} )
