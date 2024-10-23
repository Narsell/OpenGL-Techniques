#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {

	class TextureTest : public Test {

	public:

		TextureTest();
		~TextureTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnCleanUp() override;

	private:

		Shader shader;

		glm::vec3 m_TranslationA;
		glm::vec3 m_TranslationB;
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;

		// Vertex buffer data
		// Position (x, y), color vec 4 (r, g, b, a), texture coords (x, y), texture ID (0 = none)
		float m_VertexData[36] = 
		{
			0.0f,   0.0f,   0.2f, 0.6f, 0.9f, 1.0f, 0.0f, 0.0f, 1.0f, //0
			100.0f, 0.0f,   0.2f, 0.6f, 0.9f, 1.0f, 1.0f, 0.0f, 1.0f, //1
			100.0f, 100.0f, 0.2f, 0.6f, 0.9f, 1.0f, 1.0f, 1.0f, 1.0f, //2
			0.0f,   100.0f, 0.2f, 0.6f, 0.9f, 1.0f, 0.0f, 1.0f, 1.0f, //3
		};

		unsigned int m_VertexCount = 4;
		unsigned int m_ElementsPerVertex = 9;

		//Defines the order in which we want the vertices to be read in OpenGL.
		unsigned int m_Indices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};

		VertexBuffer m_VertexBuffer;
		VertexBufferLayout m_VertexLayout;
		VertexArray m_VertexArray;
		IndexBuffer m_IndexBuffer;
		Texture m_Texture;
		unsigned int m_TextureSlot = 0;
	};
}
