#pragma once

#include "Core.h"
#include "core\Timestep.h"
#include "events\Event.h"

namespace SOMEENGINE
{
	class SOME_API Layer
	{
	protected:
		std::string _DebugName;

	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

	public:
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(Timestep ts) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return _DebugName; }
	};
}

