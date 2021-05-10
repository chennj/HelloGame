#pragma once

#include "entt.hpp"
#include "stuff/core/Timestep.h"

namespace SOMEENGINE
{
	using registry_et = entt::basic_registry<entt::entity>;

	class Entity;
	class Scene
	{
		friend class Entity;
	private:
		//entt::registry _Registry;
		registry_et _Registry;

	public:
		Scene();
		~Scene();

	public:
		void OnUpdate(Timestep ts);
		Entity CreateEntity(const std::string& name = std::string());
	};
}