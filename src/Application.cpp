#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cassert>
#include <vector>

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

#include "tests/ClearColorTest.h"
#include "tests/TextureTest.h"
#include "tests/BatchRenderingTest.h"

enum class APP_STATE : uint8_t
{
    NONE = 0,
    SELECTION_MENU = 1,
    IN_TEST = 2
};

int main(void)
{
    GLFWwindow* window;
    APP_STATE appState;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
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

    Renderer renderer;

    ImGui::CreateContext();

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

    test::ClearColorTest clearColorTest;
    //test::TextureTest textureTest;
    test::BatchRenderingTest batchRenderingTest;

    std::vector<test::Test*> testList;
    testList.reserve(4);
    testList.push_back(&clearColorTest);
    //testList.push_back(&textureTest);
    testList.push_back(&batchRenderingTest);

    appState = APP_STATE::SELECTION_MENU;
    test::Test* selectedTest = nullptr;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (appState == APP_STATE::IN_TEST && selectedTest)
        {
            //Selected test update and render updates
            selectedTest->OnUpdate(0.f);
            selectedTest->OnRender();
            selectedTest->OnImGuiRender();
            
            //If the back button was pressed, go back and cleanup
            if (selectedTest->IsBackButtonPressed())
            {
                appState = APP_STATE::SELECTION_MENU;
                selectedTest->OnCleanUp();
                selectedTest = nullptr;
            }
        }
        else if (appState == APP_STATE::SELECTION_MENU)
        {
            //Iterate through the test list and use the selected one.
            ImGui::Begin("Test List");
            for (test::Test* test : testList)
            {
                if (ImGui::Button(test->GetName().c_str(), ImVec2(90, 35)))
                {
                    std::cout << test->GetName() << " selected!\n";
                    appState = APP_STATE::IN_TEST;
                    selectedTest = test;
                    break;
                }
            }
            ImGui::End();
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