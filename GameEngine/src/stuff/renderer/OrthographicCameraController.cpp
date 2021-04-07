#include "sopch.h"
#include "OrthographicCameraController.h"
#include "stuff\core\Input.h"
#include "stuff\core\KeyCodes.h"

namespace SOMEENGINE
{
	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:
		_Camera(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel),
		_AspectRatio(aspectRatio), 
		_Rotation(rotation)
	{
	}

	void OrthographicCameraController::OnUpdate(Timestep ts)
	{
		if (Input::IsKeyPressed(SE_KEY_A))
			_CameraPosition.x -= _CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(SE_KEY_D))
			_CameraPosition.x += _CameraTranslationSpeed * ts;

		if (Input::IsKeyPressed(SE_KEY_W))
			_CameraPosition.y += _CameraTranslationSpeed * ts;
		else if (Input::IsKeyPressed(SE_KEY_S))
			_CameraPosition.y -= _CameraTranslationSpeed * ts;

		if (_Rotation)
		{
			if (Input::IsKeyPressed(SE_KEY_Q))
				_CameraRotation -= _CameraRotationSpeed * ts;
			if (Input::IsKeyPressed(SE_KEY_E))
				_CameraRotation += _CameraRotationSpeed * ts;

			_Camera.SetRotation(_CameraRotation);
		}

		_Camera.SetPosition(_CameraPosition);

		// ʹͼ���ڷŴ�����Сʱ���ƶ����ƽ��
		_CameraTranslationSpeed = _ZoomLevel;
	}

	void OrthographicCameraController::OnEvent(Event & e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>	(SE_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>	(SE_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
	}

	bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent & e)
	{
		_ZoomLevel -= e.GetYOffset() * 0.5f;
		_ZoomLevel = std::max(_ZoomLevel, 0.25f);
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
		return false;
	}

	bool OrthographicCameraController::OnWindowResized(WindowResizeEvent & e)
	{
		_AspectRatio = (float)e.GetWidth()/(float)e.GetHeight();
		_Camera.SetProjection(-_AspectRatio * _ZoomLevel, _AspectRatio * _ZoomLevel, -_ZoomLevel, _ZoomLevel);
		return false;
	}
}