#pragma once

#include "glm/glm.hpp"

namespace SOMEENGINE
{
	class Camera
	{
	private:
		glm::mat4 _Projection;

	public:
		Camera(const glm::mat4& projection)
			:_Projection(projection) {}

	public:
		const glm::mat4& GetProjection()const { return _Projection; }
	};
}