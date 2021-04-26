#pragma once

#include "Core.h"
#include "Layer.h"

namespace SOMEENGINE
{
	class SOME_API LayerStack
	{
	private:
		std::vector<Layer*> _Layers;
		unsigned int _LayerInsertIndex = 0;

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
		std::vector<Layer*>::reverse_iterator rbegin() { return _Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return _Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return _Layers.begin(); }
		std::vector<Layer*>::const_iterator end() const { return _Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return _Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return _Layers.rend(); }
	};
}

