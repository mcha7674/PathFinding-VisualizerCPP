#pragma once

#include "Core.h"
#include "Layer.h"

#include <vector>

namespace GLCore {

	/*The LayerStack is essetially a wrapper over its 
	vector of layers private attribute

	Owned by Application
	*/
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack(); // deletes all layers in layerstack

		void PushLayer(Layer* layer);
		// Overlayes get pushed to second half of layerstack
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer); // Will pop layer from m_Layers but not delete it
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers; // The Actual layer stack
		uint32_t m_LayerInsertIndex = 0;
	};

}