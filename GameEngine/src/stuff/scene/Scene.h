#pragma once

#include "entt.hpp"
#include "stuff/core/Timestep.h"

namespace SOMEENGINE
{
	using registry_et = entt::basic_registry<entt::entity>;

	class Scene
	{
	private:
		//entt::registry _Registry;
		registry_et _Registry;

	public:
		Scene();
		~Scene();

	public:
		void OnUpdate(Timestep ts);
		entt::entity CreateEntity();

		// tmp
		registry_et& Reg() { return _Registry; }
	};
}