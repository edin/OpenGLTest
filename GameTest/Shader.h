#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class ShaderProgram {
private:
	GLint id;

	void ValidateShader(GLuint shaderId) {
		char message[512];
		GLint status;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(shaderId, 512, nullptr, message);
			std::cerr << "ShaderProgram::ValidateShader: " << message << "\n";
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
private:
	void ValidateProgram(GLuint programId) {
		char message[512];
		GLint status;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);
		if (!status) {
			glGetProgramInfoLog(programId, 512, nullptr, message);
			std::cerr << "Shader::ValidateProgram: " << message << "\n";
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
	}

	~Shader() {
		glUseProgram(0);
		glDeleteProgram(id);
	}

	void Use()
	{
		glUseProgram(id);
	}

	void EndUse()
	{
		glUseProgram(0);
	}

	void setMatrix(const std::string& name, const glm::mat4& mat) {
		glUniformMatrix4fv(GetLocation(name), 1, GL_FALSE, &mat[0][0]);
	}

	void setFloat(const std::string& name, float value) {
		glUniform1f(GetLocation(name), value);
	}

	void setInt(const std::string& name, int value) {
		glUniform1i(GetLocation(name), value);
	}

	void setBool(const std::string& name, bool value) {
		glUniform1i(GetLocation(name), value);
	}
};