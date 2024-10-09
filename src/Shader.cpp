#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <GL/glew.h>

Shader::Shader(const std::string& filepath)
	:m_FilePath(filepath), m_RendererId(0)
{
    //Parses a single shader file into a VertexSource shader and a FragmentSource shader
    ShaderProgramSource shaderSources = ParseShader(filepath);
    //Compiles each shader separately and returns a shader program index for OpenGL to use.
    m_RendererId = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);

 }

Shader::~Shader()
{
    glDeleteProgram(m_RendererId);
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
    //We get the file into a stream obj
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX = 0,
        FRAGMENT = 1
    };

    std::string line;
    std::stringstream shaderSource[2];
    ShaderType shaderType = ShaderType::NONE;

    //We parse through the file
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                shaderType = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                shaderType = ShaderType::FRAGMENT;
            }
        }
        else
        {
            //We insert line by line into shaderSource with the appropriate index.
            shaderSource[(int)shaderType] << line << '\n';
        }
    }

    return { shaderSource[0].str(), shaderSource[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
    //Creates an empty shader object returns an id that openGL can refer to. https://docs.gl/gl4/glCreateShader
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    //Sets the src into the shader identified by the given id. https://docs.gl/gl4/glShaderSource
    glShaderSource(id, 1, &src, nullptr);
    //Compiles the source code that have been stored in the shader identified by the given id. https://docs.gl/gl4/glCompileShader
    glCompileShader(id);

    //This blob of code retrieves the compilation status and prints if it failed.
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "FAILED TO COMPILE!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    //We return the compiled shader id.
    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    //Creates program and compiles shaders, getting the openGL id from each one.
    unsigned int program = glCreateProgram();
    unsigned int vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //Attaches shaders to created program
    glAttachShader(program, vertexShaderId);
    glAttachShader(program, fragmentShaderId);
    //Links and validates
    glLinkProgram(program);
    glValidateProgram(program);

    //We delete the shaders since we don't need them anymore (they're part of the program now so we can free that memory)
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return program;
}


void Shader::Bind() const
{
    glUseProgram(m_RendererId);
}

void Shader::Unbind() const
{
    glUseProgram(0);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
    glUniform1i(GetUniformLocation(name), value);

}

unsigned int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    {
        return m_UniformLocationCache[name];
    }

    int location = glGetUniformLocation(m_RendererId, name.c_str());
    if (location == -1)
    {
        std::cout << "Warning: Uniform '" << name << "' does not exist!\n";
    } 
    m_UniformLocationCache[name] = location;
    return location;
}
