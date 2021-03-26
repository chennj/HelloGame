#pragma once

#include "Core.h"
#include "Layer.h"

namespace SOMEENGINE
{
	class SOME_API LayerStack
	{
	private:
		std::vector<Layer*> _Layers;
		std::vector<Layer*>::iterator _LayerInsert;

	public:
		LayerStack();
		~LayerStack();

	public:
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return _Layers.begin(); }
		std::vector<Layer*>::iterator end() { return _Layers.end(); }
	};
}

