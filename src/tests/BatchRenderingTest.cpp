#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BatchRenderingTest.h"
#include "Renderer.h"

test::BatchRenderingTest::BatchRenderingTest()
	:Test("Batch Rendering"), 
    shader("res/shaders/Basic.shader"), 
    m_Translation(0.0f, 0.0f, 0.0f),
    //Setting up orthographic projection matrix (to map a 3D space into a 2D) and sets the bounding limits of our window
    //and converts everything into normalized device coordinates (-1 to 1 in every axis)
    m_ProjMatrix(glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f)),
    //Defines translation of the view (camera)
    m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_VertexBuffer(m_VertexData, m_VertexCount* m_ElementsPerVertex * sizeof(float)),
    m_IndexBuffer(m_Indices, 12),
    m_Texture("res/textures/meteor.png")
{
    m_VertexLayout.Push<float>(2);
    m_VertexLayout.Push<float>(2);

    m_VertexArray.AddBuffer(m_VertexBuffer, m_VertexLayout);

    //Binding texture to a slot
    m_Texture.Bind(m_TextureSlot);
    //Sending texture slot to the shader through a uniform
    shader.SetUniform1i("u_Texture", m_TextureSlot);
}

test::BatchRenderingTest::~BatchRenderingTest()
{
}

void test::BatchRenderingTest::OnUpdate(float deltaTime)
{
}

void test::BatchRenderingTest::OnRender()
{
    Renderer renderer;

    //Render translation A
    {
        //Defines translation of the actual vertex in device coords.
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
        //PVM because OpenGL uses column major
        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
        shader.Bind();
        //We then send this MVP matrix to the shader through a uniform
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(m_VertexArray, m_IndexBuffer, shader);
    }
}

void test::BatchRenderingTest::OnImGuiRender()
{
	Test::OnImGuiRender();

    ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 960.f);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void test::BatchRenderingTest::OnCleanUp()
{
    m_VertexArray.Unbind();
    m_VertexBuffer.Unbind();
    m_IndexBuffer.Unbind();
    shader.Unbind();
    m_Texture.Unbind();
}
