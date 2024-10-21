#include "Test.h"

void test::Test::OnImGuiRender()
{
	ImGui::Begin("Test Parameters");
}

const bool test::Test::IsBackButtonPressed()
{
	const bool isBackPressed = ImGui::Button("Back", ImVec2(50, 25));
	ImGui::End();
	return isBackPressed;
}
