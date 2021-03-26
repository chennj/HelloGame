#pragma once

#include "stuff\Layer.h"

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
	};
}