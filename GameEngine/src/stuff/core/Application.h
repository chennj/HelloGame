#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "stuff\events\Event.h"
#include "stuff\events\ApplicationEvent.h"

#include "stuff\core\Timestep.h"

#include "stuff\imgui\ImGuiLayer.h"

namespace SOMEENGINE
{
	class Application
	{
	private:
		Scope<Window> _Window;
		ImGuiLayer* _ImGuiLayer;
		bool _Running = true;
		LayerStack _LayerStack;

		float _LastFrameTime = 0.0f;
		bool _Minimized = false;

	private:
		static Application* s_Instance;

	public:
		Application();
		virtual ~Application();

	public:
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline Window& GetWindow() const { return *_Window; }
		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	};

	// To be define in CLIENT
	Application* CreateApplication();
}

