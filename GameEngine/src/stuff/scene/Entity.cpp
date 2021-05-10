#include "sopch.h"
#include "Entity.h"

namespace SOMEENGINE
{
	Entity::Entity(entt::entity entityHandle, Scene * scene)
		:_EntityHandle(entityHandle), _Scene(scene)
	{
	}
}
