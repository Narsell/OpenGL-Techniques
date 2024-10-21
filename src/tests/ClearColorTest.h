#pragma once

#include "Test.h"

namespace test
{
	class ClearColorTest : public Test
	{
	public:
		ClearColorTest();
		~ClearColorTest();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
		void OnCleanUp() override;

	private:

		float m_ClearColor[4];

	};
}