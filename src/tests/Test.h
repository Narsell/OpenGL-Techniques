#pragma once

#include <sstream>
#include "imgui/imgui.h"

namespace test
{
	struct Defaults
	{
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

	/*
	* Base test interface
	* Important: Test::OnImGuiRender must be called on all derived classes that implement this function.
	*/
	class Test
	{
	public:
		Test() {}

		Test(const std::string& name)
		:m_name(name)
		{}

		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() 
		{
			ImGui::Begin("Test Parameters");
		}
		virtual void OnCleanUp() {}

		inline const std::string& GetName() const { return m_name; };

		virtual const bool IsBackButtonPressed() 
		{
			const bool isBackPressed = ImGui::Button("Back", ImVec2(50, 25));
			ImGui::End();
			return isBackPressed;
		}

	protected:

		Defaults defaults;

	private:

		std::string m_name;

	};
}