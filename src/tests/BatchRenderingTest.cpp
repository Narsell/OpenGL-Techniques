#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "BatchRenderingTest.h"
#include "Renderer.h"

test::BatchRenderingTest::BatchRenderingTest()
	:Test("Batch Rendering"), 
    shader("res/shaders/Basic.shader"), 
    m_Translation(0.0f, 50.0f, 0.0f),
    // Setting up orthographic projection matrix (to map a 3D space into a 2D) and sets the bounding limits of our window
    // and converts everything into normalized device coordinates (-1 to 1 in every axis)
    m_ProjMatrix(glm::ortho(0.0f, 640.f, 0.0f, 480.f, -1.0f, 1.0f)),
    // Defines translation of the view (camera)
    m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_MeteorTexture("res/textures/meteor.png"),
    m_ShipTexture("res/textures/ship.png")
{
    // Vertex buffer data
    // Position (x, y), color vec 4 (r, g, b, a), texture coords (x, y), texture ID (0 = none)
    float vertexData [] =
    {
        100.0f, 0.0f,   0.2f, 0.6f, 0.9f, 1.0f, 0.0f, 0.0f, 1.0f, //0
        200.0f, 0.0f,   0.2f, 0.6f, 0.9f, 1.0f, 1.0f, 0.0f, 1.0f, //1
        200.0f, 100.0f, 0.2f, 0.6f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, //2
        100.0f, 100.0f, 0.2f, 0.6f, 0.9f, 1.0f, 0.0f, 1.0f, 1.0f, //3

        300.0f, 0.0f,   1.0f, 0.9f, 0.2f, 1.0f, 0.0f, 0.0f, 2.0f, //4
        400.0f, 0.0f,   1.0f, 0.9f, 0.2f, 1.0f, 1.0f, 0.0f, 2.0f, //5
        400.0f, 100.0f, 1.0f, 0.9f, 0.2f, 1.0f, 1.0f, 1.0f, 2.0f, //6
        300.0f, 100.0f, 1.0f, 0.9f, 0.2f, 1.0f, 0.0f, 1.0f, 2.0f  //7
    };

    // Create and bind VAO (Vertex Array Object)
    glCreateVertexArrays(1, &m_QuadVA);
    glBindVertexArray(m_QuadVA);

    // Generate, bind and set vertex buffer data.
    glCreateBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    // Setting up vertex attributes
    // position attribute
    glEnableVertexArrayAttrib(m_QuadVA, 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), 0);
    // color attribute
    glEnableVertexArrayAttrib(m_QuadVA, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(2 * sizeof(float)));
    // text coord attribute
    glEnableVertexArrayAttrib(m_QuadVA, 2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(6 * sizeof(float)));
    // text ID attribute
    glEnableVertexArrayAttrib(m_QuadVA, 3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (const void*)(8 * sizeof(float)));

    unsigned int indices [12] =
    {
        0, 1, 2,
        2, 3, 0,

        4, 5, 6,
        6, 7, 4
    };

    glCreateBuffers(1, &m_QuadIB);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_QuadIB);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    //Getting uniform location and setting uniform value to array of integers containing texture slots
    glUseProgram(shader.GetRendererId());
    int texturesUniformLoc = glGetUniformLocation(shader.GetRendererId(), "u_Textures");
    constexpr uint32_t textureCount = 2;
    int textureSamplers[textureCount] = { 0, 1 };
    glUniform1iv(texturesUniformLoc, textureCount, textureSamplers);

}

test::BatchRenderingTest::~BatchRenderingTest()
{
}

void test::BatchRenderingTest::OnUpdate(float deltaTime)
{
}

void test::BatchRenderingTest::OnRender()
{
    //Defines translation of the actual vertex in device coords.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
    //PVM because OpenGL uses column major
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;

    //Bind shader
    glUseProgram(shader.GetRendererId());

    //Get MVP uniform location and send data to that uniform location.
    int mvpUniformLoc = glGetUniformLocation(shader.GetRendererId(), "u_MVP");
    glUniformMatrix4fv(mvpUniformLoc, 1, GL_FALSE, &mvp[0][0]);

    //Bind texture ID to texture slot/unit
    glBindTextureUnit(0, m_MeteorTexture.GetRendererId());
    glBindTextureUnit(1, m_ShipTexture.GetRendererId());

    //Bind VAO and draw!
    glBindVertexArray(m_QuadVA);

    const uint32_t indexCount = 12;
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

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
    //Unbind Vertex Array - VAO
    glBindVertexArray(0);
    //Unbind Vertex Buffer - VB
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //Unbind Index Buffer - IB
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //Unbind Shader
    glUseProgram(0);
    //Unbind textures
    glBindTexture(GL_TEXTURE_2D, 0);
}
