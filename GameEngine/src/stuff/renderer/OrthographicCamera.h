#pragma once

#include "glm/glm.hpp"

namespace SOMEENGINE
{
	class OrthographicCamera
	{
	private:
		////////////////////////////////////
		// Camera = ͶӰ�任���� * ��ͼ�任����
		////////////////////////////////////
		// ͶӰ�任����
		glm::mat4 _ProjectionMatrix;
		// ��ͼ�任����
		glm::mat4 _ViewMatrix;
		// ͶӰ��ͼ�任����
		glm::mat4 _ViewProjectionMatrix;
		// Camera λ������
		glm::vec3 _Position = { 0.0, 0.0, 0.0 };
		
		// Camera ��ת�Ƕ�
		float _Rotation = 0.0f;

	public:
		OrthographicCamera(float left, float right, float bottom, float top);

	public:
		const glm::vec3& GetPosition()const { return _Position; }
		void SetPosition(const glm::vec3& position) 
		{ 
			_Position = position; 
			RecalculateViewMatrix(); 
		}

		float GetRotation() const { return _Rotation; }
		void SetRotation(float rotation)
		{ 
			_Rotation = rotation;
			RecalculateViewMatrix();
		}

		const glm::mat4& GetViewProjectionMatrix()const { return _ViewProjectionMatrix; }
		const glm::mat4& GetViewMatrix()const { return _ViewMatrix; }
		const glm::mat4& GetProjectionMatrix()const { return _ProjectionMatrix; }

	private:
		void RecalculateViewMatrix();
	};
}