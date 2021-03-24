#pragma once

#include "Core.h"
#include "events/Event.h"

namespace SOMEENGINE
{
	class SOME_API Application
	{
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
	};

	// To be define in CLIENT
	Application* CreateApplication();
}

