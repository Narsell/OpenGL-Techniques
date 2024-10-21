#pragma once

#include <sstream>
#include "imgui/imgui.h"

namespace test
{
	struct Defaults
	{
		float clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
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
		virtual void OnImGuiRender();
		virtual void OnCleanUp() {}

		inline const std::string& GetName() const { return m_name; };

		virtual const bool IsBackButtonPressed();

	protected:

		Defaults defaults;

	private:

		std::string m_name;

	};
}