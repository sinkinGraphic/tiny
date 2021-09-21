#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "FileHelper.h"
#include "Paths.h"
#include "Shader.h"
#include "stb_image.h"

void OnFrameBufferResized(GLFWwindow* Window, int Width, int Height)
{
    glViewport(0, 0, Width, Height);
}

void ProcessInput(GLFWwindow* Window)
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

    GLFWwindow* Window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
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
            0.5f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
    };

    float VertexColor[] = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f
    };

    float uv[] = {
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,
            0.0f, 1.0f
    };

    unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
    };


    //VBO
    unsigned int VBO[3];
    glGenBuffers(3, VBO);

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

    glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uv), uv, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //wireframe mode
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    //load image
    int width, height, nrChannels;
    const std::string PathString = Tiny::Paths::ProjectDir() + "Resources/container.jpg";
    const char* ImagePath = PathString.c_str();
    unsigned char* Data = stbi_load(ImagePath, &width, &height, &nrChannels, 0);
    unsigned int texture;
    glGenTextures(1, &texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(Data);

    const std::string PathString2 = Tiny::Paths::ProjectDir() + "Resources/awesomeface.png";
    const char* ImagePath2 = PathString2.c_str();
    stbi_set_flip_vertically_on_load(true);
    unsigned char* Data2 = stbi_load(ImagePath2, &width, &height, &nrChannels, 0);
    unsigned int texture2;
    glGenTextures(1, &texture2);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data2);

    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(Data2);

    while (!glfwWindowShouldClose(Window))
    {
        //input
        ProcessInput(Window);

        //render
        glClearColor(.2f, .3f, .3f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        Shader.Use();
        Shader.SetInt("Texture", 0);
        Shader.SetInt("Texture2", 1);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
        //glGenBuffers(1, &EBO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //switch buffer
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}