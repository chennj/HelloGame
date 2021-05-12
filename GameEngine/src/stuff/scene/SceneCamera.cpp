#include "sopch.h"
#include "SceneCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace SOMEENGINE
{
	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	SceneCamera::~SceneCamera()
	{
	}

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft		= -_OrthographicSize * _AspectRatio * 0.5f;
		float orthoRight	=  _OrthographicSize * _AspectRatio * 0.5f;
		float orthoBottom	= -0.5f * _OrthographicSize;
		float orthoTop		=  0.5f * _OrthographicSize;

		_Projection = glm::ortho(orthoLeft, orthoRight, orthoBottom, orthoTop, _OrthographicNear, _OrthographicFar);
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		_OrthographicSize = size;
		_OrthographicNear = nearClip;
		_OrthographicFar = farClip;

		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		SE_CORE_ASSERT((width > 0 && height > 0), "SceneCamera::SetViewportSize Failed");

		_AspectRatio = (float)width / (float)height;

		RecalculateProjection();
	}
}
