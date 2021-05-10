#include "sopch.h"

#include "Scene.h"
#include "stuff/renderer/Renderer2D.h"
#include "Components.h"
#include "glm\glm.hpp"
#include "Entity.h"

namespace SOMEENGINE
{
	static void DoMaths(const glm::mat4& transform)
	{
		// ...
	}

	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{

	}

	Scene::Scene()
	{
#if entt的简单用法
		struct MeshComponent 
		{
			bool tmp;
		};

		struct TransformComponent
		{
			glm::mat4 Transform;

			TransformComponent() = default;
			TransformComponent(const TransformComponent&) = default;
			TransformComponent(const glm::mat4& transform) :Transform(transform) {}

			operator glm::mat4&() { return Transform; }
			operator const glm::mat4&() const { return Transform; }
		};

		entt::entity entity = _Registry.create();
		_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));

		_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>();

		if (_Registry.valid(entity))
			TransformComponent& transform = _Registry.get<TransformComponent>(entity);

		auto view = _Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);
			// ...
		}

		auto group = _Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto&[transform, mesh] = group.get< TransformComponent, MeshComponent>(entity);

			//Renderer::Submit(mesh, transform);
		}
#endif
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		auto group = _Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			auto&[transform, sprite] = group.get< TransformComponent, SpriteRendererComponent>(entity);
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { _Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>();
		tag.Tag = name.empty() ? "Entity" : name;

		return entity;
	}
}