#include "sopch.h"

#include "Scene.h"
#include "stuff/renderer/Renderer2D.h"
#include "Components.h"
#include "glm\glm.hpp"
#include "Entity.h"

namespace SOMEENGINE
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(Timestep ts)
	{
		// Update scripts
		{
			_Registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) 
			{
				// instead of virtual function
				//if (!nsc.Instance)
				//{
				//	nsc.InstantiateFunction();
				//	nsc.Instance->_Entity = Entity{ entity,this };
				//	nsc.OnCreateFunction(nsc.Instance);
				//}

				//nsc.OnUpdateFunction(nsc.Instance, ts);

				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->_Entity = Entity{ entity,this };
					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}

		// Render 2D
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{
			auto view = _Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto[transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}
		if (mainCamera)
		{
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			auto group = _Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				auto[transform, sprite] = group.get< TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		_ViewportWidth = width;
		_ViewportHeight = height;

		// Resize our non-fixedAspectRatio cameras
		auto view = _Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
			{
				cameraComponent.Camera.SetViewportSize(width, height);
			}
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