#pragma once

#include "Scene.h"

namespace SOMEENGINE
{
	class Entity
	{
	private:
		entt::entity _EntityHandle{ entt::null };
		Scene* _Scene = nullptr;

	public:
		Entity() = default;
		Entity(entt::entity entityHandle, Scene* scene);
		Entity(const Entity& other) = default;

	public:
		template<typename T>
		bool HasComponent()
		{
			return _Scene->_Registry.any_of<T>(_EntityHandle);
		}

		template<typename T, typename...Args>
		T& AddComponent(Args&&... args)
		{
			SE_CORE_ASSERT(!HasComponent<T>(), "Entity already has component");
			return _Scene->_Registry.emplace<T>(_EntityHandle, std::forward<Args>(args)...);
		}

		template<typename T>
		T& GetComponent()
		{
			SE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component");
			return _Scene->_Registry.get<T>(_EntityHandle);
		}

		template<typename T>
		void RemoveComponent()
		{
			SE_CORE_ASSERT(HasComponent<T>(), "Entity doesn't have component");
			_Scene->_Registry.remove<T>(_EntityHandle);
		}

		operator bool()const { return _EntityHandle != entt::null; }
	};

}