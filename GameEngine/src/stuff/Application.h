#pragma once

#include "Core.h"
#include "events/Event.h"
#include "events\ApplicationEvent.h"
#include "Window.h"

namespace SOMEENGINE
{
	class SOME_API Application
	{
	private:
		std::unique_ptr<Window> _Window;
		bool _Running = true;

	public:
		Application();
		virtual ~Application();

	public:
		void Run();

	public:
		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);
	};

	// To be define in CLIENT
	Application* CreateApplication();
}

