#pragma once

#include "glm/glm.hpp"

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
		glm::mat4 Transform = glm::mat4(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& transform) :Transform(transform) {}

		operator glm::mat4&() { return Transform; }
		operator const glm::mat4&() const { return Transform; }
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