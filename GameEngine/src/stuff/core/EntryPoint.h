#pragma once

#ifdef SE_PLATFORM_WINDOWS

extern SOMEENGINE::Application* SOMEENGINE::CreateApplication();

int main(int argc, char** args)
{
	SOMEENGINE::Log::Init();

	SE_PROFILE_BEGIN_SESSION("Startup", "SEProfile-Startup.json");
	auto app = SOMEENGINE::CreateApplication();
	SE_PROFILE_END_SESSION();

	SE_PROFILE_BEGIN_SESSION("Startup", "SEProfile-Runtime.json");
	app->Run();
	SE_PROFILE_END_SESSION();

	SE_PROFILE_BEGIN_SESSION("Startup", "SEProfile-Shutdown.json");
	delete app;
	SE_PROFILE_END_SESSION();
}

#endif