#pragma once

#include "stuff\Layer.h"
#include "stuff\events\ApplicationEvent.h"
#include "stuff\events\MouseEvent.h"
#include "stuff\events\KeyEvent.h"

namespace SOMEENGINE
{
	class SOME_API ImGuiLayer : public Layer
	{
	private:
		float _Time = 0.0f;

	public:
		ImGuiLayer();
		~ImGuiLayer();

	public:
		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);

	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(MouseMovedEvent& e);
		bool OnMouseScrolledEvent(MouseScrolledEvent& e);

		bool OnKeyPressedEvent(KeyPressedEvent& e);
		bool OnKeyReleasedEvent(KeyReleasedEvent& e);
		bool OnKeyTypedEvent(KeyTypedEvent& e);

		bool OnWindowResizedEvent(WindowResizeEvent& e);
	};
}