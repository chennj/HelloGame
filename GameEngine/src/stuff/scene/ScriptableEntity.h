#pragma once

#include "Entity.h"

namespace SOMEENGINE
{
	class ScriptableEntity
	{
		friend class Scene;
	private:
		Entity _Entity;

	public:
		virtual ~ScriptableEntity() {}
		template<typename T>
		T& GetComponent()
		{
			return _Entity.GetComponent<T>();
		}

	protected:
		virtual void OnCreate() {};
		virtual void OnDestroy() {};
		virtual void OnUpdate(Timestep ts) {};
	};
}
