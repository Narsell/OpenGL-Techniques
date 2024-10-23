#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TextureTest.h"
#include "Renderer.h"

test::TextureTest::TextureTest()
	:Test("Texture Test"), 
    m_Shader("res/shaders/Basic.shader"), 
    m_TranslationA(200.0f, 25.0f, 0.0f),
    m_TranslationB(400.0f, 25.0f, 0.0f),
    //Setting up orthographic projection matrix (to map a 3D space into a 2D) and sets the bounding limits of our window
    //and converts everything into normalized device coordinates (-1 to 1 in every axis)
    m_ProjMatrix(glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f)),
    //Defines translation of the view (camera)
    m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_VertexBuffer(m_VertexData, m_VertexCount* m_ElementsPerVertex * sizeof(float)),
    m_IndexBuffer(m_Indices, 6),
    m_Texture("res/textures/meteor.png")
{
    m_VertexLayout.Push<float>(2);
    m_VertexLayout.Push<float>(4);
    m_VertexLayout.Push<float>(2);
    m_VertexLayout.Push<float>(1);

    m_VertexArray.AddBuffer(m_VertexBuffer, m_VertexLayout);

    //Sending texture slot to the shader through a uniform (array of integers)
    m_Shader.Bind();
    int textureSamplers[1] = { 0 };
    m_Shader.SetUniform1iv("u_Textures", 1, textureSamplers);
}

test::TextureTest::~TextureTest()
{
}

void test::TextureTest::OnUpdate(float deltaTime)
{
}

void test::TextureTest::OnRender()
{
    Renderer renderer;

    //Binding texture to m_TextureSlot slot
    //glBindTextureUnit(m_TextureSlot, m_Texture.GetRendererId()); //gl4 way
    m_Texture.Bind(m_TextureSlot);

    m_Shader.Bind();
    //Render translation A
    {
        //Defines translation of the actual vertex in device coords.
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);
        //PVM because OpenGL uses column major
        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
        //We then send this MVP matrix to the shader through a uniform
        m_Shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(m_VertexArray, m_IndexBuffer, m_Shader);
    }
    //Render translation B
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);
        glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;
        m_Shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(m_VertexArray, m_IndexBuffer, m_Shader);
    }
}

void test::TextureTest::OnImGuiRender()
{
	Test::OnImGuiRender();

    ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.f);
    ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.f);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void test::TextureTest::OnCleanUp()
{
    m_VertexArray.Unbind();
    m_VertexBuffer.Unbind();
    m_IndexBuffer.Unbind();
    m_Shader.Unbind();
    m_Texture.Unbind();
}
