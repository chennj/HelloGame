#pragma once

#include "glm/glm.hpp"

namespace SOMEENGINE
{
	class Camera
	{
	protected:
		glm::mat4 _Projection = glm::mat4(1.0f);

	public:
		Camera(const glm::mat4& projection)
			:_Projection(projection) {}
		Camera() = default;
		virtual ~Camera() = default;

	public:
		const glm::mat4& GetProjection()const { return _Projection; }
	};
}