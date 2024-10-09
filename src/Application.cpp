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
#include "Shader.h"
#include "Texture.h"

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

    //Vertex buffer data
    //Position x, Position y, tex coord x, tex coord y
    float vertexData[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, //0
         0.5f, -0.5f, 1.0f, 0.0f, //1
         0.5f,  0.5f, 1.0f, 1.0f, //2
        -0.5f,  0.5f, 0.0f, 1.0f  //3
    };

    // We define the order in which we want the vertices to be read in OpenGL.
    unsigned int indices[]  = {
        0, 1, 2,
        2, 3, 0
    };

    unsigned int vertexCount = 4;
    unsigned int elementsInVertex = 4;
    VertexBuffer vb(vertexData, vertexCount * elementsInVertex * sizeof(float));
    

    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);

    VertexArray va;
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

    //Creating texture from path and binding it as the active texture
    Texture texture("res/textures/ship.png");
    unsigned int textureSlot = 0;
    texture.Bind(textureSlot);
    shader.SetUniform1i("u_Texture", textureSlot);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    //Blending config
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);

    float redValue = 0.0f;
    float redIncrement = 0.05f;
    //Set the value of the 'u_Color' uniform variable in the shader. Thus passing data from the CPU to the GPU.

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        shader.Bind();
        shader.SetUniform4f("u_Color", redValue, 0.3f, 0.8f, 1.0f);

        renderer.Draw(va, ib, shader);

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

    glfwTerminate();
    return 0;
}