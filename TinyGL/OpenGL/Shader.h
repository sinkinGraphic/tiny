#pragma once

#include <exception>
#include "glad/glad.h"

namespace Tiny
{
    class ShaderCompileError : public std::exception
    {
    public:
        [[nodiscard]] const char* what() const noexcept override;

        explicit ShaderCompileError(const char* ErrorInfo)
                : Error(ErrorInfo)
        {
        };

    public:
        const char* Error;
    };

    class Shader final
    {
    public:
        Shader();

        ~Shader();

    public:
        void Compile(GLenum ShaderType, const char* ShaderSource);

        void Link();

        void Use();

    public:
        void SetInt(const char* ParamName,int Value);

    private:
        unsigned int VertexShader = 0;
        unsigned int FragmentShader = 0;
        unsigned int ShaderProgram = 0;
    };
}