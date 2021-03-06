#pragma once

#include "stuff/renderer/Camera.h"

namespace SOMEENGINE
{
	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };

	private:
		float _OrthographicSize = 10.0f;
		float _OrthographicNear = -1.0f, _OrthographicFar = 1.0f;

		float _PerspectiveFOV = glm::radians(45.0f);
		float _PerspectiveNear = 0.01f, _PerspectiveFar = 1000.0f;

		float _AspectRatio = 0.0f;

		ProjectionType _ProjectionType = ProjectionType::Orthographic;

	public:
		SceneCamera();
		virtual ~SceneCamera();

	private:
		void RecalculateProjection();

	public:
		void SetOrthographic(float size, float nearClip, float farClip);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetViewportSize(uint32_t width, uint32_t height);

		ProjectionType GetProjectionType() const { return _ProjectionType; }
		void SetProjectionType(ProjectionType type) { _ProjectionType = type; RecalculateProjection(); }

		float GetOrthographicSize()const { return _OrthographicSize; }
		void SetOrthographicSize(float size) { _OrthographicSize = size; RecalculateProjection(); }
		float GetOrthographicNearClip()const { return _OrthographicNear; }
		void SetOrthographicNearClip(float nearClip) { _OrthographicNear = nearClip; RecalculateProjection();}
		float GetOrthographicFarClip()const { return _OrthographicFar; }
		void SetOrthographicFarClip(float farClip) { _OrthographicFar = farClip; RecalculateProjection(); }

		float GetPerspectiveVerticalFOV()const { return _PerspectiveFOV; }
		void SetPerspectiveVerticalFOV(float verticalFOV) { _PerspectiveFOV = verticalFOV; RecalculateProjection(); }
		float GetPerspectiveNearClip()const { return _PerspectiveNear; }
		void SetPerspectiveNearClip(float nearClip) { _PerspectiveNear = nearClip; RecalculateProjection(); }
		float GetPerspectiveFarClip()const { return _PerspectiveFar; }
		void SetPerspectiveFarClip(float farClip) { _PerspectiveFar = farClip; RecalculateProjection(); }
	};
}