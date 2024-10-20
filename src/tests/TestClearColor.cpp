#include "TestClearColor.h"
#include <GL/glew.h>
#include "imgui/imgui.h"

test::TestClearColor::TestClearColor()
	:Test("Clear Color"), m_ClearColor{0.2f, 0.3f, 0.8, 1.0f}
{
}

test::TestClearColor::~TestClearColor()
{
}

void test::TestClearColor::OnUpdate(float deltaTime)
{
}

void test::TestClearColor::OnRender()
{
	glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]);
	glClear(GL_COLOR_BUFFER_BIT);
}

void test::TestClearColor::OnImGuiRender()
{
	ImGui::ColorEdit4("Clear Color", m_ClearColor);
}

void test::TestClearColor::OnCleanUp()
{
	glClearColor(defaults.clearColor[0], defaults.clearColor[1], defaults.clearColor[2], defaults.clearColor[3]);
}
