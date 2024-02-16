#include "Shader.h"
#include "GL/glew.h"

#include <fstream>
#include <string>
#include <sstream>

#include <iostream>




GLuint vertexShader, fragmentShader, tessEvelShader, tessControlShader, geometryShader;
GLint success;
GLchar infoLog[1024];
Shader::Shader(const std::string& filepath , int id)
	: m_FilePath(filepath) , m_RendererID(0), isVertexEnabled(false), isFragmentEnabled(false), isTessEvalEnabled(false), isTessControlEnabled(false), isGeometryEnabled(false)
{
    m_RendererID = m_RendererID + id;
    std::cout << m_RendererID << std::endl;
	CompileShader(filepath);
}

Shader::~Shader()
{

}
ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1, EVAL = 2, CONTROL = 3, GEOMETRY = 4,
    };
    ShaderType type = ShaderType::NONE;
    std::string line;
    std::stringstream ss[5];
    while (getline(stream, line)) {

        if (line.find("#shader") != std::string::npos) {
            if (line.find("vertex") != std::string::npos) {
                std::cout << "found vertex" << std::endl;
                isVertexEnabled = true;
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos) {
                std::cout << "found frag"<< std::endl;
                isFragmentEnabled = true;
                type = ShaderType::FRAGMENT;
            }
            else if (line.find("TessEval") != std::string::npos) {
                std::cout << "found eval" << std::endl;
                isTessEvalEnabled = true;
                type = ShaderType::EVAL;
            }
            else if (line.find("TessControl") != std::string::npos) {
                std::cout << "found control" << std::endl;
                isTessControlEnabled = true;
            type = ShaderType::CONTROL;

            }
            else if (line.find("Geometry") != std::string::npos) {
                std::cout << "found Geometry" << std::endl;
                isGeometryEnabled = true;
                type = ShaderType::GEOMETRY;
            }
        }
        else {
            ss[(int)type] << line << '\n';
        }

    }
    return { ss[0].str(), ss[1].str() ,ss[2].str(),ss[3].str(),ss[4].str() };


}
bool Shader::CompileShader(const std::string& filepath) {
    ShaderProgramSource source = ParseShader(filepath); //load in the shader info from a file and creates a struct from it

    const char* vertexSrc = source.VertexSource.c_str();
    const char* fragSrc = source.FragmentSource.c_str();
    const char* tessEvelSrc = source.TesselationEval.c_str();
    const char* tessControlSrc = source.TesselationControl.c_str();
    const char* GeometrySrc = source.GeometrySource.c_str();

    // Build and compile our shader program
    // Vertex shader
    if (isVertexEnabled) {
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexSrc, NULL);
        glCompileShader(vertexShader);
        // Check for compile time errors

        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(vertexShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    if (isFragmentEnabled) {
        // Fragment shader
        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragSrc, NULL);
        glCompileShader(fragmentShader);

        // Check for compile time errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(fragmentShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    if (isTessEvalEnabled) {
        // Tess Evel shader
        tessEvelShader = glCreateShader(GL_TESS_EVALUATION_SHADER);
        glShaderSource(tessEvelShader, 1, &tessEvelSrc, NULL);
        glCompileShader(tessEvelShader);

        // Check for compile time errors
        glGetShaderiv(tessEvelShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(tessEvelShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::TESSEVAL::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    if (isTessControlEnabled) {
        // Tess Control shader
        tessControlShader = glCreateShader(GL_TESS_CONTROL_SHADER);
        glShaderSource(tessControlShader, 1, &tessControlSrc, NULL);
        glCompileShader(tessControlShader);

        // Check for compile time errors
        glGetShaderiv(tessControlShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(tessControlShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::TESSCONTROL::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    if (isGeometryEnabled) {
        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        glShaderSource(geometryShader, 1, &GeometrySrc, NULL);
        glCompileShader(geometryShader);

        // Check for compile time errors
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }


    // Link shaders
    GLuint shaderProgram = glCreateProgram();
    m_RendererID = shaderProgram;
    if (isVertexEnabled) {
        glAttachShader(shaderProgram, vertexShader);
    }
    if(isFragmentEnabled) {
        glAttachShader(shaderProgram, fragmentShader);
    }
    if(isTessEvalEnabled) {
        glAttachShader(shaderProgram, tessEvelShader);
    }
    if(isTessControlEnabled) {
        glAttachShader(shaderProgram, tessControlShader);
    }
    if(isGeometryEnabled) {
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

void Shader::Bind() const
{
    glUseProgram(m_RendererID);
}

void Shader::UnBind() const
{
    glUseProgram(0);
}


void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformedLocation(name), value);
}
void Shader::SetUniform1f(const std::string& name, float v0)
{
    glUniform1f(GetUniformedLocation(name), v0);
}
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformedLocation(name), v0, v1, v2, v3);
}
void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
    glUniform3f(GetUniformedLocation(name), v0, v1, v2);
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
    glUniformMatrix4fv(GetUniformedLocation(name),1,GL_FALSE,&matrix[0][0]);
}

unsigned int Shader::GetUniformedLocation(const std::string& name)
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