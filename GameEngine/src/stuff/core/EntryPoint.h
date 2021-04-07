#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SOMEENGINE::Application* SOMEENGINE::CreateApplication();

int main(int argc, char** args)
{
	SOMEENGINE::Log::Init();
	SE_CORE_WARN("Initialized Logger!");
	SE_INFO("Welcome to some engine!");

	auto app = SOMEENGINE::CreateApplication();
	app->Run();
	delete app;
}

#endif