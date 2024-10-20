#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "TextureTest.h"
#include "Renderer.h"

test::TextureTest::TextureTest()
	:Test("Texture Test"), 
    shader("res/shaders/Basic.shader"), 
    translationA(200.0f, 0.0f, 0.0f),
    translationB(400.0f, 0.0f, 0.0f),
    vb(vertexData, vertexCount* elementsInVertex * sizeof(float)),
    ib(indices, 6),
    texture("res/textures/meteor.png")
{
    layout.Push<float>(2);
    layout.Push<float>(2);

    va.AddBuffer(vb, layout);

    //Setting up orthographic projection matrix (to map a 3D space into a 2D) and sets the bounding limits of our window
    //and converts everything into normalized device coordinates (-1 to 1 in every axis)
    proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
    //Defines translation of the view (camera)
    view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

    //Binding texture to a slot
    texture.Bind(textureSlot);
    //Sending texture slot to the shader through a uniform
    shader.SetUniform1i("u_Texture", textureSlot);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
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

    //Render translation A
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
    //Render translation B
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
        glm::mat4 mvp = proj * view * model;
        shader.Bind();
        shader.SetUniformMat4f("u_MVP", mvp);
        renderer.Draw(va, ib, shader);
    }
}

void test::TextureTest::OnImGuiRender()
{
	Test::OnImGuiRender();

    ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.f);
    ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.f);

    ImGuiIO& io = ImGui::GetIO();
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
}

void test::TextureTest::OnCleanUp()
{
    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();
    texture.Unbind();
}
