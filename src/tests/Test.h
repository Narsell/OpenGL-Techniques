#pragma once

#include <sstream>
#include "imgui/imgui.h"

namespace test
{
	struct Defaults
	{
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

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
		virtual void OnImGuiRender() {}
		virtual void OnCleanUp() {}

		inline const std::string& GetName() const { return m_name; };

		virtual const bool IsBackButtonPressed() 
		{
			return ImGui::Button("Back", ImVec2(50, 25));
		}

	protected:

		Defaults defaults;

	private:

		std::string m_name;

	};
}