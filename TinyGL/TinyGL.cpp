#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

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

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    while (!glfwWindowShouldClose(Window))
    {
        //input
        ProcessInput(Window);

        //render
        glClearColor(.2f,.3f,.3f,1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        //switch buffer
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }
    return 0;
}