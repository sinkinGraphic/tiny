#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "FileHelper.h"
#include "Paths.h"

void OnFrameBufferResized(GLFWwindow *Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow *Window)
{
    if (glfwGetKey(Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(Window, true);
    }
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow *Window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
    if (Window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(Window);

    //set callback
    glfwSetFramebufferSizeCallback(Window, &OnFrameBufferResized);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(&glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    while (!glfwWindowShouldClose(Window))
    {
        //input
        ProcessInput(Window);

        //render
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        //switch buffer
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    //Vertex Shader
    std::string VertexShaderSource;
    const std::string ShaderFilePath = Tiny::Paths::ProjectDir() + "Shader/VertexShaderSource.glsl";
    if (Tiny::FileHelper::LoadFileToString(VertexShaderSource, ShaderFilePath))
    {
        std::cout << VertexShaderSource << std::endl;
    }

    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f
    };
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int VertexShader;
    VertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *VertexShaderSourceAddress = VertexShaderSource.data();
    glShaderSource(VertexShader, 1, &VertexShaderSourceAddress, nullptr);
    glCompileShader(VertexShader);

    //Fragment Shader
    std::string FragmentShaderSource;
    const std::string FragmentShaderPath = Tiny::Paths::ProjectDir() + "Shader/FragmentShader.glsl";

    if (Tiny::FileHelper::LoadFileToString(FragmentShaderSource, FragmentShaderPath))
    {
        std::cout << FragmentShaderSource << std::endl;
    }

    unsigned int FragmentShader;
    FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *FragmentShaderSourceAddress = FragmentShaderSource.data();
    glShaderSource(FragmentShader, 1, &FragmentShaderSourceAddress, nullptr);
    glCompileShader(FragmentShader);

    //Shader Program
    unsigned int ShaderProgram;
    ShaderProgram = glCreateProgram();
    glAttachShader(ShaderProgram, VertexShader);
    glAttachShader(ShaderProgram, FragmentShader);
    glLinkProgram(ShaderProgram);

    glUseProgram(ShaderProgram);

    glDeleteShader(VertexShader);
    glDeleteShader(FragmentShader);
    return 0;
}