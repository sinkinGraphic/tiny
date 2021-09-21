#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Shader.h"

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

    Tiny::Shader Shader;

    //Vertex Shader
    std::string VertexShaderSource;
    const std::string ShaderFilePath = Tiny::Paths::ProjectDir() + "Shader/VertexShader.glsl";
    if (Tiny::FileHelper::LoadFileToString(VertexShaderSource, ShaderFilePath))
    {
        std::cout << VertexShaderSource << std::endl;
    }
    Shader.Compile(GL_VERTEX_SHADER, VertexShaderSource.data());

    //Fragment Shader
    std::string FragmentShaderSource;
    const std::string FragmentShaderPath = Tiny::Paths::ProjectDir() + "Shader/FragmentShader.glsl";
    if (Tiny::FileHelper::LoadFileToString(FragmentShaderSource, FragmentShaderPath))
    {
        std::cout << FragmentShaderSource << std::endl;
    }
    Shader.Compile(GL_FRAGMENT_SHADER, FragmentShaderSource.data());
    Shader.Link();

    float vertices[] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.0f, 0.5f, 0.0f,
    };

    float VertexColor[] = {
            1.f, 0.f, 0.f,
            0.f, 1.f, 0.f,
            0.f, 0.f, 1.f
    };

    //VBO
    unsigned int VBO[2];
    glGenBuffers(1, &VBO[0]);
    glGenBuffers(1, &VBO[1]);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexColor), VertexColor, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    unsigned int indices[] = {
            0, 1, 2,
            0, 1, 5
    };

    //unsigned int EBO;
    //glGenBuffers(1, &EBO);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while (!glfwWindowShouldClose(Window))
    {
        //input
        ProcessInput(Window);

        //render
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        Shader.Use();

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        //glGenBuffers(1, &EBO);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //switch buffer
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}