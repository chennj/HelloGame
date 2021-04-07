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
			glm::rotate(glm::mat4(1.0f),glm::radians(_Rotation), glm::vec3(0,0,1)/*绕Z轴旋转，(0,1,0)则是绕Y轴旋转*/);
		// 矩阵转置
		_ViewMatrix = glm::inverse(transform);
		// 重新计算视图投影变换矩阵
		_ViewProjectionMatrix = _ProjectionMatrix * _ViewMatrix;
	}
}