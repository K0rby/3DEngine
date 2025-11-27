cmake_minimum_required(VERSION 3.10)

project(GraphicsProgramming)

set(CMAKE_CXX_STANDARD 17)

# Adiciona os arquivos fonte
add_executable(GraphicsApp
    src/main.cpp
    External/src/glad.c
)

# Inclui diretórios para o compilador achar headers
target_include_directories(GraphicsApp PRIVATE
    External/include
)

# Linka as bibliotecas necessárias
target_link_libraries(GraphicsApp
    glfw
    dl
    GL
)
