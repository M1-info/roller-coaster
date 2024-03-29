#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <glm/glm.hpp>

#include <string>

#include <unordered_map>

#include <iostream>
#include <sstream>
#include <fstream>

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

class Shader
{
public:
	Shader(const std::string &filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	void SetUniform1i(const std::string &name, int value);
	void SetUniform1f(const std::string &name, float value);
	void SetUniform3f(const std::string &name, float v0, float v1, float v2);
	void SetUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string &name, const glm::mat4 matrix);

	bool HasUniform(const std::string &name);

private:
	unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader);
	ShaderProgramSource ParseShader();
	unsigned int CompileShader(unsigned int type, const std::string &source);
	int GetUniformLocation(const std::string &name) const;

private:
	std::string m_Filename;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
};

#endif // SHADER_H