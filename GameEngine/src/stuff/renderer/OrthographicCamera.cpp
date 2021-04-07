#include "sopch.h"
#include "OrthographicCamera.h"

#include "glm/gtc/matrix_transform.hpp"

namespace SOMEENGINE
{
	OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
		:
		_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
		_ViewMatrix(1.0f)
	{
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
	void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
	{
		_ProjectionMatrix = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
	void OrthographicCamera::RecalculateViewMatrix()
	{
		glm::mat4 transform = 
			glm::translate(glm::mat4(1.0f), _Position) 
			* 
			glm::rotate(glm::mat4(1.0f),glm::radians(_Rotation), glm::vec3(0,0,1)/*��Z����ת��(0,1,0)������Y����ת*/);
		// ����ת��
		_ViewMatrix = glm::inverse(transform);
		// ���¼�����ͼͶӰ�任����
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
}