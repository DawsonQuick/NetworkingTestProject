#pragma once
#include <string>
#include <unordered_map>
#include "./../vendor/glm/glm.hpp"
struct ShaderProgramSourceV2 {
	std::string VertexSource;
	std::string FragmentSource;
	std::string TesselationEval;
	std::string TesselationControl;
	std::string GeometrySource;
};

class ShaderV2
{
private:
	unsigned int m_RendererID;
	std::unordered_map<std::string, int > m_UniformLocationCache;
public:
	ShaderV2(int id, const std::string& VertexShader, const std::string& FragmentShader, const std::string& TesselationControlShader, const std::string& TessellationEvalShader, const std::string& GeometryShader);
	~ShaderV2();

	ShaderProgramSourceV2 ParseShader(const std::string& VertexShader, const std::string& FragmentShader, const std::string& TesselationControlShader, const std::string& TessellationEvalShader, const std::string& GeometryShader);
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
	bool CompileShader(const std::string& VertexShader, const std::string& FragmentShader, const std::string& TesselationControlShader, const std::string& TessellationEvalShader, const std::string& GeometryShader);
	unsigned int GetUniformedLocation(const std::string& name);
};