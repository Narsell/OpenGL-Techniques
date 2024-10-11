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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


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
    float vertexData[] = 
    {
        0.0f,  0.0f, 0.0f, 0.0f, //0
        100.0f, 0.0f, 1.0f, 0.0f, //1
        100.0f, 100.0f, 1.0f, 1.0f, //2
        0.0f, 100.0f, 0.0f, 1.0f  //3
    };

    // We define the order in which we want the vertices to be read in OpenGL.
    unsigned int indices[] = 
    {
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

    //Loading vertex and fragment shaders
    Shader shader("res/shaders/Basic.shader");
    shader.Bind();

    //Setting up orthographic projection matrix (to map a 3D space into a 2D) and sets the bounding limits of our window
    //and converts everything into normalized device coordinates (-1 to 1 in every axis)
    glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    //Defines translation of the view (camera)
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    //Creating texture from path and binding it as the active texture
    Texture texture("res/textures/meteor.png");
    unsigned int textureSlot = 0;
    texture.Bind(textureSlot);
    //Sending texture slot to the shader through a uniform
    shader.SetUniform1i("u_Texture", textureSlot);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 130";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    //Blending config
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);

    glm::vec3 translationA(200.0f, 0.0f, 0.0f);
    glm::vec3 translationB(400.0f, 0.0f, 0.0f);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        {
            //Defines translation of the actual vertex in device coords.
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
            //PVM because OpenGL uses column major
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            //We then send this MVP matrix to the shader through a uniform
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
        }

        {
            //Defines translation of the actual vertex in device coords.
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
            //PVM because OpenGL uses column major
            glm::mat4 mvp = proj * view * model;

            shader.Bind();
            //We then send this MVP matrix to the shader through a uniform
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, ib, shader);
        }

        {
            ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.f);
            ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.f);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();
    return 0;
}