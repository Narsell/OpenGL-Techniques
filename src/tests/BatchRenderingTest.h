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

	class BatchRenderingTest: public Test {

	public:

		BatchRenderingTest();
		~BatchRenderingTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnCleanUp() override;

	private:

		Shader shader;

		glm::vec3 m_Translation;
		glm::mat4 m_ProjMatrix;
		glm::mat4 m_ViewMatrix;

		//Vertex buffer data
		//Position x, Position y, tex coord x, tex coord y
		float m_VertexData[32] = 
		{
			100.0f, 0.0f,   0.0f, 0.0f, //0
			200.0f, 0.0f,   1.0f, 0.0f, //1
			200.0f, 100.0f, 1.0f, 1.0f, //2
			100.0f, 100.0f, 0.0f, 1.0f, //3

			300.0f, 0.0f,   0.0f, 0.0f, //4
			400.0f, 0.0f,   1.0f, 0.0f, //5
			400.0f, 100.0f, 1.0f, 1.0f, //6
			300.0f, 100.0f, 0.0f, 1.0f  //7
		};

		unsigned int m_VertexCount = 8;
		unsigned int m_ElementsPerVertex = 4;

		//Defines the order in which we want the vertices to be read in OpenGL.
		unsigned int m_Indices[12] =
		{
			0, 1, 2,
			2, 3, 0,

			4, 5, 6,
			6, 7, 4
		};

		VertexBuffer m_VertexBuffer;
		VertexBufferLayout m_VertexLayout;
		VertexArray m_VertexArray;
		IndexBuffer m_IndexBuffer;
		Texture m_Texture;
		unsigned int m_TextureSlot = 0;
	};
}
