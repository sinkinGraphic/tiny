#include <iostream>
#include "Shader.h"

namespace Tiny
{
    const char *ShaderCompileError::what() const
    {
        return Error;
    }

    Shader::Shader()
    {
        ShaderProgram = glCreateProgram();
    }

    void Shader::Compile(GLenum ShaderType, const char *ShaderSource)
    {
        unsigned int *pShader = nullptr;
        switch (ShaderType)
        {
            case GL_VERTEX_SHADER:
            {
                pShader = &VertexShader;
                break;
            }
            case GL_FRAGMENT_SHADER:
            {
                pShader = &FragmentShader;
                break;
            }
            default:
            {
                ShaderCompileError ShaderCompileException("Wrong shader type");
                throw (ShaderCompileException);
            }
        }
        *pShader = glCreateShader(ShaderType);
        glShaderSource(*pShader, 1, &ShaderSource, nullptr);
        glCompileShader(*pShader);

        int success = 0;
        char infoLog[512];
        glGetShaderiv(*pShader, GL_COMPILE_STATUS, &success);
        if (success == 0)
        {
            glGetShaderInfoLog(*pShader, 512, nullptr, infoLog);
            ShaderCompileError ShaderCompileException(infoLog);
            throw (ShaderCompileException);
        }
        glAttachShader(ShaderProgram, *pShader);
    }

    Shader::~Shader()
    {
        glDeleteShader(VertexShader);
        glDeleteShader(FragmentShader);
        glDeleteProgram(ShaderProgram);
    }

    void Shader::Use()
    {
        if (VertexShader == 0)
        {
            std::cerr << "Vertex Shader not valid" << std::endl;
        }
        if (FragmentShader == 0)
        {
            std::cerr << "Fragment Shader not valid" << std::endl;
        }
        glUseProgram(ShaderProgram);
    }

    void Shader::Link()
    {
        glLinkProgram(ShaderProgram);
    }

    void Shader::SetInt(const char* ParamName, int Value)
    {
        unsigned int ParamLocation = glGetUniformLocation(ShaderProgram,ParamName);
        glUniform1i(ParamLocation,Value);
    }

}