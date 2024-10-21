#include "ClearColorTest.h"
#include <GL/glew.h>
#include "imgui/imgui.h"

test::ClearColorTest::ClearColorTest()
	:Test("Clear Color"), m_ClearColor{0.2f, 0.3f, 0.8f, 1.0f}
{
}

test::ClearColorTest::~ClearColorTest()
{
}

void test::ClearColorTest::OnUpdate(float deltaTime)
{
}

void test::ClearColorTest::OnRender()
{
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void test::ClearColorTest::OnImGuiRender()
{
	Test::OnImGuiRender();
	ImGui::ColorEdit4("Clear Color", m_ClearColor);
}

void test::ClearColorTest::OnCleanUp()
{
	glClearColor(defaults.clearColor[0], defaults.clearColor[1], defaults.clearColor[2], defaults.clearColor[3]);
}
