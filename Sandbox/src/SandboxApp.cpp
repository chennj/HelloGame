#include "sopch.h"

#include <Some.h>

class Sandbox : public SOMEENGINE::Application
{
public:
	Sandbox()
	{

	}
	~Sandbox()
	{

	}
};

SOMEENGINE::Application* SOMEENGINE::CreateApplication()
{
	return new Sandbox();
}