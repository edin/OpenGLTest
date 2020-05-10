#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <GL/glew.h>
#include <glm/glm.hpp>

class Shader {
private:
	GLuint id;
private:
	void ValidateShader(GLuint shaderId) {
		char message[512];
		GLint status;
		glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
		if (!status) {
			glGetShaderInfoLog(shaderId, 512, nullptr, message);
			std::cerr << "Shader::ValidateShader: " << message << "\n";
		}
	}

	void ValidateProgram(GLuint programId) {
		char message[512];
		GLint status;
		glGetProgramiv(programId, GL_LINK_STATUS, &status);
		if (!status) {
			glGetProgramInfoLog(programId, 512, nullptr, message);
			std::cerr << "Shader::ValidateProgram: " << message << "\n";
		}
	}

	GLuint GetShader(GLenum shaderType, const std::string& source) {
		auto shader = glCreateShader(shaderType);
		const GLchar* src = source.c_str();
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		ValidateShader(shader);
		return shader;
	}

	GLint GetLocation(const std::string& name) {
		return glGetUniformLocation(id, name.c_str());
	}

public:
	Shader(std::string vertexShader, std::string fragmentShader) {
		GLuint vertexShaderID = GetShader(GL_VERTEX_SHADER, vertexShader);
		GLuint fragmentShaderID = GetShader(GL_FRAGMENT_SHADER, fragmentShader);
		
		id = glCreateProgram();
		glAttachShader(id, vertexShaderID);
		glAttachShader(id, fragmentShaderID);
		glLinkProgram(id);
		ValidateProgram(id);
	}

	~Shader() {
		glDeleteShader(id);
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