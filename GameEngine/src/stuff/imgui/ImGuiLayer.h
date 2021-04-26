#pragma once

#include "stuff\core\Layer.h"
#include "stuff\events\ApplicationEvent.h"
#include "stuff\events\MouseEvent.h"
#include "stuff\events\KeyEvent.h"

namespace SOMEENGINE
{
	class SOME_API ImGuiLayer : public Layer
	{
	private:
		float _Time = 0.0f;
		bool _BlockEvents = false;

	public:
		ImGuiLayer();
		~ImGuiLayer();

	public:
		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnImGuiRender()override;
		virtual void OnEvent(Event& e)override;

		void Begin();
		void End();

		void BlockEvents(bool block) { _BlockEvents = block; }
	};
}