#pragma once

#include "TypeAliases.h"
#include "Logger.h"

namespace Nuts {
    class ShaderProgram {
    private:
        GLint id;

        void ValidateShader(GLuint shaderId) {
            char message[512];
            GLint status;
            glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
            if (!status) {
                glGetShaderInfoLog(shaderId, 512, nullptr, message);
                Logger::Error("ShaderProgram::ValidateShader", message);
            }
        }
    public:
        ShaderProgram(GLenum shaderType, const std::string& source) {
            id = glCreateShader(shaderType);
            const GLchar* src = source.c_str();
            glShaderSource(id, 1, &src, nullptr);
            glCompileShader(id);
            ValidateShader(id);
        }

        ~ShaderProgram() {
            glDeleteShader(id);
        }

        GLint GetShaderID() { return id; }
    };

    class Shader {
    private:
        GLuint id;
        GLint MVPLocation;
    private:
        void ValidateProgram(GLuint programId) {
            char message[512];
            GLint status;
            glGetProgramiv(programId, GL_LINK_STATUS, &status);
            if (!status) {
                glGetProgramInfoLog(programId, 512, nullptr, message);
                Logger::Error("Shader::ValidateProgram", message);
            }
        }

        GLint GetLocation(const std::string& name) {
            return glGetUniformLocation(id, name.c_str());
        }

    public:
        Shader(const std::string& vertexShaderSource, const std::string& fragmentShaderSource) {
            ShaderProgram vertexShader(GL_VERTEX_SHADER, vertexShaderSource);
            ShaderProgram fragmentShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

            id = glCreateProgram();
            glAttachShader(id, vertexShader.GetShaderID());
            glAttachShader(id, fragmentShader.GetShaderID());
            glLinkProgram(id);
            ValidateProgram(id);

            glDetachShader(id, vertexShader.GetShaderID());
            glDetachShader(id, fragmentShader.GetShaderID());

            MVPLocation = GetLocation("MVP");
        }

        ~Shader() {
            glDeleteProgram(id);
        }

        void Bind()
        {
            glUseProgram(id);
        }

        void SetMVPMatrix(const glm::mat4& mat) {
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &mat[0][0]);
        }

        void SetMatrix(const std::string& name, const glm::mat4& mat) {
            glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &mat[0][0]);
        }

        void SetFloat(const std::string& name, float value) {
            glUniform1f(GetLocation(name), value);
        }

        void SetInt(const std::string& name, int value) {
            auto loc = GetLocation(name);
            glUniform1i(loc, value);
        }

        void SetBool(const std::string& name, bool value) {
            glUniform1i(GetLocation(name), value);
        }
    };
}