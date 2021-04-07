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

	public:
		ImGuiLayer();
		~ImGuiLayer();

	public:
		virtual void OnAttach()override;
		virtual void OnDetach()override;
		virtual void OnImGuiRender()override;

		void Begin();
		void End();
	};
}