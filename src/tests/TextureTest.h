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

		glm::vec3 translationA;
		glm::vec3 translationB;
		glm::mat4 proj;
		glm::mat4 view;

		unsigned int vertexCount = 4;
		unsigned int elementsInVertex = 4;

		//Vertex buffer data
		//Position x, Position y, tex coord x, tex coord y
		float vertexData[16] = 
		{
			0.0f,   0.0f,   0.0f, 0.0f, //0
			100.0f, 0.0f,   1.0f, 0.0f, //1
			100.0f, 100.0f, 1.0f, 1.0f, //2
			0.0f,   100.0f, 0.0f, 1.0f  //3
		};

		//Defines the order in which we want the vertices to be read in OpenGL.
		unsigned int indices[6] =
		{
			0, 1, 2,
			2, 3, 0
		};

		VertexBuffer vb;
		VertexBufferLayout layout;
		VertexArray va;
		IndexBuffer ib;
		Texture texture;
		unsigned int textureSlot = 0;
	};
}
