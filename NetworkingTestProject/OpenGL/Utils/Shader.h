#pragma once
#include <string>
#include <unordered_map>
#include "./../vendor/glm/glm.hpp"
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
	std::string TesselationEval;
	std::string TesselationControl;
	std::string GeometrySource;
};

class Shader
{
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int > m_UniformLocationCache;
public:
	Shader(const std::string& filepath, int id);
	~Shader();

	ShaderProgramSource ParseShader(const std::string& filepath);
	void Bind() const;
	void UnBind() const;

	//set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniform3f(const std::string& name, float v0, float v1, float v2);
	void SetUniform1f(const std::string& name, float v0);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	bool isVertexEnabled, isFragmentEnabled, isTessEvalEnabled, isTessControlEnabled, isGeometryEnabled;
	bool CompileShader(const std::string& filepath);
	unsigned int GetUniformedLocation(const std::string& name);
};