#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "SceneCamera.h"
#include "ScriptableEntity.h"

namespace SOMEENGINE
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag) :Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& transform) :Translation(transform) {}

		//operator glm::mat4&() { return Transform; }
		//operator const glm::mat4&() const { return Transform; }
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.y, { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), Rotation.z, { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color{ 1.0f,1.0f,1.0f,1.0f };

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color) :Color(color) {}
	};

	// 相机组件并不一定需要与当前视口（viewport）大小绑定
	struct CameraComponent
	{
		SceneCamera Camera;
		// 可能有多个摄像机
		bool Primary = true;
		// 固定纵横比
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct NativeScriptComponent
	{
		ScriptableEntity* Instance = nullptr;

		//std::function<void()> InstantiateFunction;
		//std::function<void()> DestroyInstanceFunction;

		//std::function<void(ScriptableEntity*)> OnCreateFunction;
		//std::function<void(ScriptableEntity*)> OnDestroyFunction;
		//std::function<void(ScriptableEntity*,Timestep)> OnUpdateFunction;

		// instead of virtual function
		ScriptableEntity* (*InstantiateScript)();
		void(*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			//InstantiateFunction = [&]() {Instance = new T(); };
			//DestroyInstanceFunction = [&]() {delete (T*)Instance; Instance = nullptr; };

			//OnCreateFunction	= [](ScriptableEntity* instance) {((T*)instance)->OnCreate(); };
			//OnDestroyFunction	= [](ScriptableEntity* instance) {((T*)instance)->OnDestroy(); };
			//OnUpdateFunction	= [](ScriptableEntity* instance, Timestep ts) {((T*)instance)->OnUpdate(ts); };

			// instead of virtual function
			InstantiateScript = []() {return static_cast<ScriptableEntity*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}