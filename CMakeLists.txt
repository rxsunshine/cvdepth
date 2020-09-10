cmake_minimum_required(VERSION 3.5.0)
project(cvdepth)

set(CMAKE_CXX_FLAGS "/std:c++17")

set(OpenCV_DIR OpenCV/cmake)
find_package(OpenCV REQUIRED)

add_library(SDL2 INTERFACE)
target_include_directories(SDL2 PUBLIC INTERFACE
    SDL2-2.0.12/include
)
target_link_libraries(SDL2 PUBLIC INTERFACE
    ../SDL2-2.0.12/lib/x64/SDL2
    ../SDL2-2.0.12/lib/x64/SDL2main
)

add_library(GLEW INTERFACE)
target_include_directories(GLEW PUBLIC INTERFACE
    glew-2.1.0/include
)
target_link_libraries(GLEW PUBLIC INTERFACE
    ../glew-2.1.0/lib/Release/x64/glew32s
)

add_library(glm INTERFACE)
target_include_directories(glm INTERFACE glm)

add_library(gfx-boilerplate STATIC
    gfx-boilerplate/gl_shader.cpp
    gfx-boilerplate/gl_prim.cpp
    gfx-boilerplate/gl_texture.cpp
    gfx-boilerplate/image.cpp
    gfx-boilerplate/stb_impl.cpp
)
target_link_libraries(gfx-boilerplate GLEW opengl32 glm)
target_include_directories(gfx-boilerplate PUBLIC gfx-boilerplate)

add_executable(main
    main.cpp
)
target_include_directories(main PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(main ${OpenCV_LIBS} SDL2 GLEW opengl32 gfx-boilerplate)