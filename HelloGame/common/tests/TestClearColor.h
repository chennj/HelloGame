#ifndef  __TEST_CLEAR_COLOR_H__
#define  __TEST_CLEAR_COLOR_H__

#include "Test.h"

namespace tests
{
	class TestClearColor : public Test
	{
	private:
		float _ClearColor[4];

	public:
		TestClearColor();
		~TestClearColor();

	public:
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}
#endif // ! __TEST_CLEAR_COLOR_H__

