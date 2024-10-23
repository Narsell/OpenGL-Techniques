#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <array>

#include "BatchRenderingTest.h"
#include "Renderer.h"

struct Vertex
{
    glm::vec2 Position;
    glm::vec4 Color;
    glm::vec2 TexCoords;
    float TexID;
};

test::BatchRenderingTest::BatchRenderingTest()
	:Test("Batch Rendering"), 
    m_Shader("res/shaders/Basic.shader"), 
    m_Translation(0.0f, 0.0f, 0.0f),
    // Setting up orthographic projection matrix (to map a 3D space into a 2D) and sets the bounding limits of our window
    // and converts everything into normalized device coordinates (-1 to 1 in every axis)
    m_ProjMatrix(glm::ortho(0.0f, 640.f, 0.0f, 480.f, -1.0f, 1.0f)),
    // Defines translation of the view (camera)
    m_ViewMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f))),
    m_MeteorTexture("res/textures/meteor.png"),
    m_ShipTexture("res/textures/ship.png")
{

    // Create and bind VAO (Vertex Array Object)
    glCreateVertexArrays(1, &m_QuadVA);
    glBindVertexArray(m_QuadVA);

    // Generate, bind and RESERVE vertex buffer data.
    glCreateBuffers(1, &m_QuadVB);
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

    // Setting up vertex attributes
    // position attribute
    glEnableVertexArrayAttrib(m_QuadVA, 0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, Position)));
    // color attribute
    glEnableVertexArrayAttrib(m_QuadVA, 1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, Color)));
    // text coord attribute
    glEnableVertexArrayAttrib(m_QuadVA, 2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, TexCoords)));
    // text ID attribute
    glEnableVertexArrayAttrib(m_QuadVA, 3);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(offsetof(Vertex, TexID)));

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
    glUseProgram(m_Shader.GetRendererId());
    int texturesUniformLoc = glGetUniformLocation(m_Shader.GetRendererId(), "u_Textures");
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

static std::array<Vertex, 4> CreateQuad(float x, float y, float textureId)
{
    float size = 100.f;

    Vertex v0;
    v0.Position = { x, y };
    v0.Color = { 0.2f, 0.6f, 0.9f, 1.0f };
    v0.TexCoords= { 0.0f, 0.0f };
    v0.TexID = textureId;

    Vertex v1;
    v1.Position = { x + size, y };
    v1.Color = { 0.2f, 0.6f, 0.9f, 1.0f };
    v1.TexCoords = { 1.0f, 0.0f };
    v1.TexID = textureId;

    Vertex v2;
    v2.Position = { x + size, y + size};
    v2.Color = { 0.2f, 0.6f, 0.9f, 1.0f };
    v2.TexCoords = { 1.0f, 1.0f };
    v2.TexID = textureId;

    Vertex v3;
    v3.Position = { x, y + size };
    v3.Color = { 0.2f, 0.6f, 0.9f, 1.0f };
    v3.TexCoords = { 0.0f, 1.0f };
    v3.TexID = textureId;

    return { v0, v1, v2, v3 };
}

void test::BatchRenderingTest::OnRender()
{
    const int quadsToRender = 2;
    const int verticesPerQuad = 4;

    //Creating quads with all their vertices
    std::array<Vertex, verticesPerQuad> q0 = CreateQuad(m_Quad1Position[0], m_Quad1Position[1], 1.0f);
    std::array<Vertex, verticesPerQuad> q1 = CreateQuad(m_Quad2Position[0], m_Quad2Position[1], 2.0f);

    //Allocating vertexData with enough memory for the two quads above
    Vertex vertexData[quadsToRender * verticesPerQuad];

    //Copy quad0 and quad1 data into vertexData. (std::array.size() is really just element count >.>)
    memcpy(vertexData, q0.data(), q0.size() * sizeof(Vertex));
    memcpy(vertexData + q0.size(), q1.data(), q1.size() * sizeof(Vertex));

    //Bind vertex buffer and set vertex data DYNAMICALLY (heck yes)
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertexData), vertexData);

    //Defines translation of the actual vertex in device coords.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
    //PVM because OpenGL uses column major (mvp is for row major)
    glm::mat4 mvp = m_ProjMatrix * m_ViewMatrix * model;

    //Bind shader
    glUseProgram(m_Shader.GetRendererId());

    //Get MVP uniform location and send data to that uniform location.
    int mvpUniformLoc = glGetUniformLocation(m_Shader.GetRendererId(), "u_MVP");
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

    ImGui::SliderFloat3("Batch translation", &m_Translation.x, 0.0f, 960.f);

    ImGui::DragFloat2("Quad 1 Position", m_Quad1Position, 5.0f);
    ImGui::DragFloat2("Quad 2 Position", m_Quad2Position, 5.0f);

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
