#pragma once

#include "stuff/core/Core.h"
#include "stuff/scene/Scene.h"
#include "stuff/scene/Entity.h"

namespace SOMEENGINE
{
	class SceneHierarchyPanel
	{
	private:
		Ref<Scene> _Context;
		Entity _SelectionContext;

	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);

	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);

	public:
		void SetContext(const Ref<Scene>& scene); 
		void OnImGuiRender();
	};
}

