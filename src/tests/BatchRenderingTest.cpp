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

    const size_t MaxQuadCount = 100;                // Arbitrary max number of quads.
    const size_t MaxVertexCount = 4 * MaxQuadCount; // Each quad has 4 vertices.
    const size_t MaxIndexCount = 6 * MaxQuadCount;  // Each quad requires 6 indices.

    glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

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

    //Construct index buffer for all possible vertices
    unsigned int indices[MaxIndexCount];
    unsigned int offset = 0;
    for (int i = 0; i < MaxIndexCount; i+=6)
    {
        indices[i + 0] = offset + 0;
        indices[i + 1] = offset + 1;
        indices[i + 2] = offset + 2;
        indices[i + 3] = offset + 2;
        indices[i + 4] = offset + 3;
        indices[i + 5] = offset + 0;

        offset += 4;
    }

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

static Vertex* CreateVertex(Vertex*& buffer, glm::vec2 pos, glm::vec2 texCoords, float textureId, glm::vec4 color)
{
    buffer->Position = pos;
    buffer->Color = color;
    buffer->TexCoords = texCoords;
    buffer->TexID = textureId;
    ++buffer;
    return buffer;
}

static Vertex* CreateQuad(Vertex*& buffer, uint32_t& indexCount, glm::vec2 position, float size, float textureId, glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f))
{
    CreateVertex(buffer, { position.x, position.y }, {0.0f, 0.0f}, textureId, color);
    CreateVertex(buffer, { position.x + size, position.y }, { 1.0f, 0.0f }, textureId, color);
    CreateVertex(buffer, { position.x + size, position.y + size }, { 1.0f, 1.0f }, textureId, color);
    CreateVertex(buffer, { position.x, position.y + size }, { 0.0f, 1.0f }, textureId, color);

    indexCount += 6;

    return buffer;
}

void test::BatchRenderingTest::OnRender()
{    
    //Define quad grid dimensions
    const uint32_t gridWidth = 5;
    const uint32_t gridHeight = 5;
    const float quadSize = 50.f;

    //Keeps track of how many indices we need
    uint32_t indexCount = 0;

    //Setup array of vertices
    std::array<Vertex, 200> vertices;

    //Points to the next vertex array mem address
    Vertex* bufferPtr = vertices.data();

    //Creates the grid
    for (int i = 0; i < gridWidth; i++)
    {
        for (int j = 0; j < gridHeight; j++)
        {
            float textureId =  (i + j) % 2 + 1.0f;
            glm::vec2 position = { quadSize * i, quadSize * j };
            CreateQuad(bufferPtr, indexCount, position, quadSize, textureId);
        }
    }

    //Two additional quads controllable by ImGUI
    CreateQuad(bufferPtr, indexCount, { m_Quad1Position[0], m_Quad1Position[1] }, quadSize, 0.0f, glm::vec4(0.2f, 0.8f, 0.2f, 1.0f));
    CreateQuad(bufferPtr, indexCount, { m_Quad2Position[0], m_Quad2Position[1] }, quadSize, 0.0f, glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

    //Bind vertex buffer and set vertex data DYNAMICALLY (heck yes)
    glBindBuffer(GL_ARRAY_BUFFER, m_QuadVB);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());

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
