#include "SceneHierarchyPanel.h"

#include "../imgui/imgui.h"
#include "stuff/scene/Components.h"

namespace SOMEENGINE
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((_SelectionContext == entity)?ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str());
		
		if (ImGui::IsItemClicked())
		{
			_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& scene)
	{
		_Context = scene;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		_Context->_Registry.each([&](auto entityID)
		{
			// ֱ��ȡ TagComponent
			//auto& tc = _Context->_Registry.get<TagComponent>(entityID);
			// ����ͨ��entity���ȡ TagComponent
			//Entity entity = { entityID, _Context.get() };
			//auto& tc = entity.GetComponent<TagComponent>();
			//ImGui::Text("%s", tc.Tag.c_str());

			Entity entity = { entityID, _Context.get() };
			DrawEntityNode(entity);
		});

		ImGui::End();
	}
}