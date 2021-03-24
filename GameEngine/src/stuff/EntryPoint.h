#pragma once

#ifdef SO_PLATFORM_WINDOWS

extern SOMEENGINE::Application* SOMEENGINE::CreateApplication();

int main(int argc, char** args)
{
	SOMEENGINE::Log::Init();
	SO_CORE_WARN("Initialized Logger!");
	SO_INFO("Welcome to some engine!");

	auto app = SOMEENGINE::CreateApplication();
	app->Run();
	delete app;
}

#endif