#include <iostream>
// glad: carregador de funções OpenGL (carrega os ponteiros das funções)
#include <glad/glad.h>
// GLFW: cria janelas e gerencia contexto/entrada
#include <GLFW/glfw3.h>

// Código GLSL do Vertex Shader (executado para cada vértice)
// Este shader pega a posição do vértice e define a posição final na tela.
const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Código GLSL do Fragment Shader (executado para cada fragmento/pixel)
// Define a cor final dos pixels do triângulo.
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
)";

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // Atualiza a área de desenho quando a janela muda de tamanho
    glViewport(0, 0, width, height);
}

int main() {
    // Inicializa a biblioteca GLFW (criação de janelas, contexto OpenGL e input)
    if (!glfwInit()) return -1;

    // Pedimos um contexto OpenGL 3.3 core profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Cria a janela onde vamos desenhar (800x600)
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangulo OpenGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Inicializa GLAD: precisa ser feito após criar o contexto GLFW
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD\n";
        return -1;
    }

    // Dados dos vértices do triângulo (x, y, z)
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Cria e configura Vertex Array Object (VAO) e Vertex Buffer Object (VBO)
    // VAO guarda o estado de configuração de atributos (como posição, cor, etc.)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO); // comece a configurar o VAO

    // carrega os dados dos vértices no VBO (memória da GPU)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Diz ao OpenGL como interpretar os dados do VBO para o atributo 0
    // aqui: 3 floats por vértice (x,y,z), sem gap entre eles
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // --- Compilação dos shaders ---
    // Compilamos o vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::VERTEX_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Compilamos o fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::FRAGMENT_SHADER_COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Linka (junta) os shaders em um programa que o GPU pode usar
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER_PROGRAM_LINKING_FAILED\n" << infoLog << std::endl;
    }

    // Shaders compilados já estão no programa; os objetos individuais podem ser deletados
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Loop de renderização: roda até a janela ser fechada
    while (!glfwWindowShouldClose(window)) {
        // Entrada: tecla ESC fecha a janela
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Limpa o buffer de cor (pinta o fundo)
        glClear(GL_COLOR_BUFFER_BIT);

        // Usa o programa de shader e desenha o triângulo
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Mostra o frame na tela e processa eventos (input, janelas)
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Limpeza: liberar recursos da GPU e encerrar GLFW
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}
