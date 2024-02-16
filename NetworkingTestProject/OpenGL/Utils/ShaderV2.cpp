#include "ShaderV2.h"
#include "GL/glew.h"

#include <fstream>
#include <string>
#include <sstream>

#include <iostream>





ShaderV2::ShaderV2( int id, const std::string& VertexShader, const std::string& FragmentShader, const std::string& TesselationControlShader, const std::string& TessellationEvalShader , const std::string& GeometryShader)
	: m_RendererID(0),isVertexEnabled(true), isFragmentEnabled(true), isTessEvalEnabled(true), isTessControlEnabled(true), isGeometryEnabled(true)
{
    if (VertexShader == " ") {
        isVertexEnabled = false;
    }
    if (FragmentShader == " ") {
        isFragmentEnabled = false;
    }
    if (TessellationEvalShader == " ") {
        isTessEvalEnabled = false;
    }
    if (TesselationControlShader == " ") {
        isTessControlEnabled = false;
    }
    if (GeometryShader == " ") {
        isGeometryEnabled = false;
    }
    m_RendererID = m_RendererID + id;
    std::cout << m_RendererID << std::endl;
	CompileShader(VertexShader,FragmentShader,TesselationControlShader,TessellationEvalShader, GeometryShader);
}

ShaderV2::~ShaderV2()
{

}
ShaderProgramSourceV2 ShaderV2::ParseShader(const std::string& VertexShader, const std::string& FragmentShader, const std::string& TesselationControlShader, const std::string& TessellationEvalShader , const std::string& GeometryShader)
{
    std::string shaders[] = { VertexShader, FragmentShader, TesselationControlShader, TessellationEvalShader ,GeometryShader };
    std::stringstream ss[5];
    

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, EVAL = 2, CONTROL = 3, GEOMETRY = 4,
    };
    for (int i = 0; i < 5; ++i) {
            std::ifstream stream(shaders[i]);
            ShaderType type = ShaderType::NONE;
            std::string line;

            while (getline(stream, line)) {

                if (line.find("#shader") != std::string::npos) {
                    if (line.find("vertex") != std::string::npos) {
                        std::cout << "found vertex" << std::endl;
                        type = ShaderType::VERTEX;
                    }
                    else if (line.find("fragment") != std::string::npos) {
                        std::cout << "found frag" << std::endl;
                        type = ShaderType::FRAGMENT;
                    }
                    else if (line.find("TessEval") != std::string::npos) {
                        std::cout << "found eval" << std::endl;
                        type = ShaderType::EVAL;
                    }
                    else if (line.find("TessControl") != std::string::npos) {
                        std::cout << "found control" << std::endl;
                        type = ShaderType::CONTROL;
                    }
                    else if (line.find("Geometry") != std::string::npos) {
                        std::cout << "found geometry" << std::endl;
                        type = ShaderType::GEOMETRY;
                    }
                }
                else {
                    ss[(int)type] << line << '\n';
                }

            }
        
    }
    return { ss[0].str(), ss[1].str() ,ss[2].str(),ss[3].str(),ss[4].str() };


}
bool ShaderV2::CompileShader(const std::string& VertexShader, const std::string& FragmentShader, const std::string& TesselationControlShader, const std::string& TessellationEvalShader, const std::string& GeometryShader) {
    ShaderProgramSourceV2 source = ParseShader(VertexShader, FragmentShader, TesselationControlShader, TessellationEvalShader, GeometryShader); //load in the shader info from a file and creates a struct from it

    const char* vertexSrc = source.VertexSource.c_str();
    const char* fragSrc = source.FragmentSource.c_str();
    const char* tessEvelSrc = source.TesselationEval.c_str();
    const char* tessControlSrc = source.TesselationControl.c_str();
    const char* geometrySrc = source.GeometrySource.c_str();

    // Build and compile our shader program
    // Vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSrc, NULL);
    glCompileShader(vertexShader);
    // Check for compile time errors
    GLint success;
    GLchar infoLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragSrc, NULL);
    glCompileShader(fragmentShader);

    // Check for compile time errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Tess Evel shader
    GLuint tessEvelShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
    glShaderSource(tessEvelShader, 1, &tessEvelSrc, NULL);
    glCompileShader(tessEvelShader);

    // Check for compile time errors
    glGetShaderiv(tessEvelShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(tessEvelShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // Tess Control shader
    GLuint tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
    glShaderSource(tessControlShader, 1, &tessControlSrc, NULL);
    glCompileShader(tessControlShader);

    // Check for compile time errors
    glGetShaderiv(tessControlShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(tessControlShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // Tess Control shader
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometryShader, 1, &geometrySrc, NULL);
    glCompileShader(geometryShader);

    // Check for compile time errors
    glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(geometryShader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }


    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    m_RendererID = shaderProgram;
    if (isVertexEnabled) {
        glAttachShader(shaderProgram, vertexShader);
    }
    if (isFragmentEnabled) {
        glAttachShader(shaderProgram, fragmentShader);
    }
    if (isTessEvalEnabled) {
        glAttachShader(shaderProgram, tessEvelShader);
    }
    if (isTessControlEnabled) {
        glAttachShader(shaderProgram, tessControlShader);
    }
    if (isGeometryEnabled) {
        glAttachShader(shaderProgram, geometryShader);
    }
    glLinkProgram(shaderProgram);
    // Check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    if (isVertexEnabled) {
        glDeleteShader(vertexShader);
    }
    if (isFragmentEnabled) {
        glDeleteShader(fragmentShader);
    }
    if (isTessEvalEnabled) {
        glDeleteShader(tessEvelShader);
    }
    if (isTessControlEnabled) {
        glDeleteShader(tessControlShader);
    }
    if (isGeometryEnabled) {
        glDeleteShader(geometryShader);
    }

    return true;
}

void ShaderV2::Bind() const
{
    glUseProgram(m_RendererID);
}

void ShaderV2::UnBind() const
{
    glUseProgram(0);
}


void ShaderV2::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformedLocation(name), value);
}
void ShaderV2::SetUniform1f(const std::string& name, float v0)
{
    glUniform1f(GetUniformedLocation(name), v0);
}
void ShaderV2::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformedLocation(name), v0, v1, v2, v3);
}
void ShaderV2::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformedLocation(name), v0, v1, v2);
}

void ShaderV2::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformedLocation(name),1,GL_FALSE,&matrix[0][0]);
}

unsigned int ShaderV2::GetUniformedLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
        return m_UniformLocationCache[name];
    }
    int location = glGetUniformLocation(m_RendererID, name.c_str());
    if (location == -1) {
        std::cout << "Warning: uniform '" << name << "'doesn't exist!" << std::endl;
    }

    m_UniformLocationCache[name] = location;
    
    return location;
}