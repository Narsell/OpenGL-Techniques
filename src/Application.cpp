#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
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

static unsigned int CompileShader(unsigned int type, const std::string& source)
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
        std::cout << "FAILED TO COMPILE!"<< std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    //We return the compiled shader id.
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
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

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Synchronize frame rate with vsync.
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "ERROR!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    //Vertex positions
    float positions[] = {
        -0.5f, -0.5f, //0
         0.5f, -0.5f, //1
         0.5f,  0.5f, //2
        -0.5f,  0.5f, //3
    };

    // We define the order in which we want the vertices to be read in OpenGL.
    unsigned int indices[]  = {
        0, 1, 2,
        2, 3, 0
    };

    VertexBuffer vb(positions, 4 * 2 * sizeof(float));
    
    VertexArray va;

    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    //Parses a single shader file into a VertexSource shader and a FragmentSource shader
    ShaderProgramSource shaderSources = ParseShader("res/shaders/Basic.shader");

    //Compiles each shader separately and returns a shader program index for OpenGL to use.
    unsigned int shaderProgram = CreateShader(shaderSources.VertexSource, shaderSources.FragmentSource);
    glUseProgram(shaderProgram);

    //Get the uniform location of 'u_Color'
    int uniformLocation = glGetUniformLocation(shaderProgram, "u_Color");
    assert(uniformLocation != -1);

    va.Unbind();
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    float redValue = 0.0f;
    float redIncrement = 0.05f;
    //Set the value of the 'u_Color' uniform variable in the shader. Thus passing data from the CPU to the GPU.

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glUniform4f(uniformLocation, redValue, 0.3f, 0.8f, 1.0f);

        va.Bind();
        ib.Bind();

        GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

        if (redValue > 1.0f)
        {
            redIncrement = -0.05f;
        }
        else if (redValue < 0.0f)
        {
            redIncrement = 0.05f;
        }

        redValue += redIncrement;

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}