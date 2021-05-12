#pragma once

#include "stuff/renderer/Camera.h"

namespace SOMEENGINE
{
	class SceneCamera : public Camera
	{
	private:
		float _OrthographicSize = 10.0f;
		float _OrthographicNear = -1.0f, _OrthographicFar = 1.0f;

		float _AspectRatio = 0.0f;

	public:
		SceneCamera();
		virtual ~SceneCamera();

	private:
		void RecalculateProjection();

	public:
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetViewportSize(uint32_t width, uint32_t height);

		float GetOrthographicSize()const { return _OrthographicSize; }
		void SetOrthographicSize(float size) { _OrthographicSize = size; RecalculateProjection(); }
	};
}