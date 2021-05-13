#include "SceneHierarchyPanel.h"

#include "../imgui/imgui.h"
#include "glm/gtc/type_ptr.hpp"
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

	void SceneHierarchyPanel::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;

			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, tag.c_str(), sizeof(buffer));
			if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Transform"))
			{
				auto& transform = entity.GetComponent<TransformComponent>().Transform;
				ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);

				ImGui::TreePop();
			}
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
			// 直接取 TagComponent
			//auto& tc = _Context->_Registry.get<TagComponent>(entityID);
			// 或则通过entity间接取 TagComponent
			//Entity entity = { entityID, _Context.get() };
			//auto& tc = entity.GetComponent<TagComponent>();
			//ImGui::Text("%s", tc.Tag.c_str());

			Entity entity = { entityID, _Context.get() };
			DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
			_SelectionContext = {};

		ImGui::End();

		ImGui::Begin("Properties");
		if (_SelectionContext)
		{
			DrawComponents(_SelectionContext);
		}
		ImGui::End();
	}
}