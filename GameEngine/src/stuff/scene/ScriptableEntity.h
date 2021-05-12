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
		template<typename T>
		T& GetComponent()
		{
			return _Entity.GetComponent<T>();
		}
	};
}
