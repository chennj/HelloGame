#pragma once

#ifdef SO_PLATFORM_WINDOWS

extern SOMEENGINE::Application* SOMEENGINE::CreateApplication();

int main(int argc, char** args)
{
	printf("Some Engine Start!\n");
	auto app = SOMEENGINE::CreateApplication();
	app->Run();
	delete app;
}

#endif