#include "Test.h"
#include "../imgui/imgui.h"

namespace tests
{
	TestMenu::TestMenu(Test*& currentTestPointer)
		: _CurrentTest(currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& test : _Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				_CurrentTest = test.second();
		}
	}
}