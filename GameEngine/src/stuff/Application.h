#pragma once

#include "Core.h"

#include "Window.h"
#include "LayerStack.h"
#include "events\Event.h"
#include "events\ApplicationEvent.h"
#include "imgui\ImGuiLayer.h"

#include "renderer\Shader.h"

namespace SOMEENGINE
{
	class SOME_API Application
	{
	private:
		std::unique_ptr<Window> _Window;
		ImGuiLayer* _ImGuiLayer;
		bool _Running = true;
		LayerStack _LayerStack;

		unsigned int _VertexArray, _VertextBuffer, _IndexBuffer;

		std::unique_ptr<Shader> _Shader;

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
	};

	// To be define in CLIENT
	Application* CreateApplication();
}

