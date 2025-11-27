cmake_minimum_required(VERSION 3.10)
project(OpenGLTriangle)

set(CMAKE_CXX_STANDARD 17)

# ------------------------------------------
# GLAD
# ------------------------------------------
add_library(glad External/src/glad.c)
target_include_directories(glad PUBLIC External/include)

# ------------------------------------------
# GLFW
# ------------------------------------------
if (WIN32)
    # Windows → usa lib pré-compilada
    set(GLFW_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/External/include)
    set(GLFW_LIBRARY ${CMAKE_SOURCE_DIR}/External/lib/glfw3.lib)

    target_include_directories(glad PUBLIC ${GLFW_INCLUDE_DIR})
else()
    # Linux → pkg-config encontra o GLFW
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    set(GLFW_LIBRARY ${GLFW_LIBRARIES})
endif()

# ------------------------------------------
# App
# ------------------------------------------
add_executable(app src/main.cpp)

target_include_directories(app PUBLIC External/include)

# Link no Windows e Linux de forma separada
if (WIN32)
    target_link_libraries(app glad ${GLFW_LIBRARY})
else()
    target_link_libraries(app glad ${GLFW_LIBRARY} dl)
endif()
