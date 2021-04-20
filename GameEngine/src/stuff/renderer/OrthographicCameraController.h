#pragma once

#include "stuff\renderer\OrthographicCamera.h"
#include "stuff\core\Timestep.h"

#include "stuff\events\ApplicationEvent.h"
#include "stuff\events\KeyEvent.h"
#include "stuff\events\MouseEvent.h"

namespace SOMEENGINE
{
	struct OrthographicCameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }

		OrthographicCameraBounds& operator()(float l, float r, float b, float t)
		{
			this->Left = l;
			this->Right = r;
			this->Bottom = b;
			this->Top = t;
			return *this;
		}
	};

	class OrthographicCameraController
	{
	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		OrthographicCamera _Camera;
		OrthographicCameraBounds _Bounds;

		bool _Rotation;
		glm::vec3 _CameraPosition = { 0.0,0.0,0.0 };
		float _CameraRotation = 0.0f;
		float _CameraTranslationSpeed = 5.0f;
		float _CameraRotationSpeed = 60.0f;

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);	// aspectratio * 2 units

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return _Camera; }
		const OrthographicCamera& GetCamera() const { return _Camera; }

		void SetZoomLevel(float level) { _ZoomLevel = level; }
		float GetZoomLevel() const { return _ZoomLevel; }

		const OrthographicCameraBounds& GetBounds()const { return _Bounds; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	};
}