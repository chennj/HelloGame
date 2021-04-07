#pragma once

#include "renderer\OrthographicCamera.h"
#include "stuff\core\Timestep.h"

#include "events\ApplicationEvent.h"
#include "events\KeyEvent.h"
#include "events\MouseEvent.h"

namespace SOMEENGINE
{
	class OrthographicCameraController
	{
	private:
		float _AspectRatio;
		float _ZoomLevel = 1.0f;
		OrthographicCamera _Camera;

		bool _Rotation;
		glm::vec3 _CameraPosition = { 0.0,0.0,0.0 };
		float _CameraRotation = 0.0f;
		float _CameraTranslationSpeed = 5.0f;
		float _CameraRotationSpeed = 180.0f;

	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);	// aspectratio * 2 units

	public:
		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		OrthographicCamera& GetCamera() { return _Camera; }
		const OrthographicCamera& GetCamera() const { return _Camera; }

		void SetZoomLevel(float level) { _ZoomLevel = level; }
		float GetZoomLevel() const { return _ZoomLevel; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	};
}