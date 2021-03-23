#ifndef __TEST_H__
#define __TEST_H__

#include <vector>
#include <functional>

namespace tests
{
	class Test
	{
	private:

	public:
		Test() {}
		virtual ~Test() {}

	public:
		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	private:
		std::vector < std::pair < std::string, std::function<Test*()> >> _Tests;
		Test*& _CurrentTest;

	public:
		TestMenu(Test*& currentTestPointer);

	public:
		void OnImGuiRender() override;

	public:
		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;
			//反射的实现方式
			_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	};
}

#endif
