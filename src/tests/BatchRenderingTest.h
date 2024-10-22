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

		GLuint m_QuadVA, m_QuadVB, m_QuadIB;
	};
}
